#pragma once
#include "../../include/state.h"
#include "../../include/move.h"
#include <iostream>

namespace HleParams {
    // read in parameters from environment variable
//    const int GREEDY_ACTION = Params::getParameterInt("GREEDY_ACTION", 1);
    const int GREEDY_ACTION = 1; // TODO replaced line above
} // namespace HleParams


struct TwoBitArray {
    TwoBitArray() {}

    uint64_t x_;

    const uint8_t get(int index) const {
        assert(index >= 0 && index < 32);
        return ((x_ >> (index * 2)) & 0x3);
    }
    void set(int index, uint64_t value) {
        assert(index >= 0 && index < 32);
        assert(value >= 0 && value < 4);
        x_ = ((x_ & ~((uint64_t) 0x3 << (index * 2))) | (value << (index * 2)));
    }
};

int card_to_index(Card c) {
    return (((int) c.color() - 1) * 5) + ((int) c.rank() - 1);
}

Card index_to_card(int i) {
    return Card(Color((i / 5) + 1), Rank((i % 5) + 1));
}

void add_to_deck(const std::vector<Card> &cards, std::vector<int> &deck_count) {
    for (auto &c : cards) deck_count[card_to_index(c)]++;
}

void remove_from_deck(const std::vector<Card> &cards, std::vector<int> &deck_count) {
    for (auto &c : cards) deck_count[card_to_index(c)]--;
}

std::vector<int> get_deck_count(State s, int id) {
    std::vector<int> deck_count = {};
    for (int k = 1; k < 6; k++) {
        for (int r = 1; r < 6; r++) {
            deck_count.push_back(r == 1 ? 3 : (r == 5 ? 1 : 2));
        }
    }
    remove_from_deck(s.get_discards(), deck_count);
    std::vector<Card> pile_cards;
    for (int k = 1; k < 6; k++) {
        for (int r = 1; r <= s.get_piles()[k]; r++) {
            Card c = Card(Color(k), Rank(r));
            pile_cards.push_back(c);
        }
    }
    remove_from_deck(pile_cards, deck_count);

    for (int i = 0; i < s.get_num_players(); i++){
        if (id == -1 || i == id) continue; // id == -1 means public
        remove_from_deck(s.get_hands()[i], deck_count);
    }
    return deck_count;
}

struct FactorizedBeliefs {
    FactorizedBeliefs(State s, int player, int num_cards) {
        p_ = player;
        std::vector<int> deck_count = get_deck_count(s, player);
        for (int k = 1; k < 6; k++) {
            for (int r = 1; r < 6; r++) {
                Card c = Card(Color(k), Rank(r));
                for (int i = 0; i< 5; i++) {
                    if (i >= num_cards) {
                        counts[i].set(card_to_index(c), 0);
                    } else {
                        counts[i].set(card_to_index(c), deck_count[card_to_index(c)]);
                    }
                }
            }
        }

        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                colorRevealed.set(i * 5 + j, 0);
                rankRevealed.set(i * 5 + j, 0);
            }
        }
        handSize = s.get_hands()[p_].size();
    }

    std::array<std::array<float, 25>, 5> get() const {
        std::array<std::array<float, 25>, 5> res;
        for (int i = 0; i < handSize; i++) {
            double total = 0;
            for (int j = 0; j < 25; j++) {
                total += counts[i].get(j);
            }
            for (int j = 0; j < 25; j++) {
                res[i][j] = counts[i].get(j) / total;
            }
        }
        for (int i = handSize; i < counts.size(); i++) {
            for (int j = 0; j < 25; j++) {
                res[i][j] = 0;
            }
        }
        return res;
    }
    
    void updateFromHint(move m, std::vector<int> c_indices, State s) {
        handSize = s.get_hands()[p_].size();
        for (int j = 0; j < 25; j++) {
            Card c = index_to_card(j);
            int card_value = m.get_type() == COL_HINT ? c.color() : c.rank();
            bool matches = m.get_type() == COL_HINT ? (m.get_color() == c.color()) : (m.get_rank() == c.rank());
            for (int i = 0; i < handSize; i ++) {
                bool found = false;
                for (int l = 0; l < c_indices.size(); l++) {
                    if (c_indices[l] == i) {
                        found = true; 
                        break;
                    }
                }
                bool consistent = found ? matches : !matches;
                if (!consistent) {
                    counts[i].set(j, 0);
                }
            }
        }
        if (m.get_type() == COL_HINT) {
            for (int i = 0; i < handSize; i ++) {
                bool found = false;
                for (int l = 0; l < c_indices.size(); l++) {
                    if (c_indices[l] == i) {
                        found = true; 
                        break;
                    }
                }
                if (found) colorRevealed.set(i * 5 + ((int) m.get_color() - 1), 1);
            }
        } else {
            for (int i = 0; i < handSize; i ++) {
                bool found = false;
                for (int l = 0; l < c_indices.size(); l++) {
                    if (c_indices[l] == i) {
                        found = true; 
                        break;
                    }
                }
                if (found) rankRevealed.set(i * 5 + ((int) m.get_rank() - 1), 1);
            }
        }
    }

    void updateFromRevealedCard(Card played_card, const std::vector<int> deck_count, State s) {
        int card_id = card_to_index(played_card);
        int remaining = deck_count[card_id];
        for (int i = 0; i < handSize; i++) {
            counts[i].set(card_id, counts[i].get(card_id) == 0 ? 0 : remaining);
        }
    }
    
    void updateFromDraw(const std::vector<int> deck_count, int card_index, State s, int num_cards) {
        handSize = s.get_hands()[p_].size();
        for (int i = card_index; i < std::min(handSize, num_cards - 1); i++) {
            for (int j = 0; j < 25; j++) {
                counts[i].set(j, counts[i + 1].get(j));
            }
            for (int j = 0; j < 5; j++) {
                colorRevealed.set(i * 5 + j, colorRevealed.get((i + 1) * 5 + j));
                rankRevealed.set(i * 5 + j, rankRevealed.get((i + 1) * 5 + j));
            }
        }
        if (handSize == num_cards) {
            // draw the new card
            for (int j = 0; j < 25; j++) {
                counts[handSize - 1].set(j, 0);
            }
            for (int i = 0; i < deck_count.size(); i++) {
                counts[handSize - 1].set(i, deck_count[i]);
            }

            // we know nothing about this new card
            for (int j = 0; j < 5; j++) {
                colorRevealed.set((handSize - 1) * 5 + j, 0);
                rankRevealed.set((handSize - 1) * 5 + j, 0);
            }
        } else {
            // no more card to draw, nil the last card
            for (int j = 0; j < 25; j++) {
                counts[handSize].set(j, 0);
            }

            // nil the last card
            for (int j = 0; j < 5; ++j) {
                colorRevealed.set(handSize * 5 + j, 0);
                rankRevealed.set(handSize * 5 + j, 0);
            }
        }
    }

    std::array<TwoBitArray, 5> counts;

    TwoBitArray colorRevealed;
    TwoBitArray rankRevealed;
    int handSize;

private:
    bool checkSum(const TwoBitArray& array, int card, int expectVal) {
        int sum = 0;
        int size = 5;
        for (int i = 0; i < size; ++i) {
            sum += array.get(5 * card + i);
        }
        if (sum != expectVal) {
            std::cout << "checkSum: " << sum << " vs "  << expectVal << std::endl;
        }
        return sum == expectVal;
    }

    int p_;
};

class HleSerializedMove {
private:
    int MaxDiscardMoves() const {
        return handSize_;
    }

    int MaxPlayMoves() const {
        return handSize_;
    }

    int MaxRevealColorMoves() const {
        return (numPlayers_ - 1) * numColors_;
    }

    int MaxRevealRankMoves() const {
        return (numPlayers_ - 1) * numRanks_;
    }

    int MaxMoves() const {
        return MaxDiscardMoves() + MaxPlayMoves() + MaxRevealColorMoves() + MaxRevealRankMoves();
    }

    int NumberCardInstance(int color, int rank) const {
        if (color < 0 || color >= numColors_ || rank < 0 || rank >= numRanks_) {
            return 0;
        }
        if (rank == 0) {
            return 3;
        } else if (rank == numRanks_ - 1) {
            return 1;
        }
        return 2;
    }

    const int numPlayers_;
    const int handSize_;

    const int numColors_ = 5;
    const int numRanks_ = 5;
    const int numCardsPerColor_ = 10;
    const int maxDeckSize_ = numColors_ * numCardsPerColor_;
    const int maxNumInfoTokens_ = 8;
    const int maxNumLifeTokens_ = 3;

    const int bitsPerCard_ = numColors_ * numRanks_;
    const int bitsPerHand_ = bitsPerCard_ * handSize_;
    const int handSectionLen_ = numPlayers_ * handSize_ * bitsPerCard_ + numPlayers_;

    const int maxRemainingDeckSize_ = maxDeckSize_ - numPlayers_ * handSize_;
    const int boardSectionLen_ = (maxRemainingDeckSize_
                                  + numColors_ * numRanks_
                                  + maxNumInfoTokens_
                                  + maxNumLifeTokens_);

    const int numMoveTypes_ = 4;
    const int lastActionSectionLen_ = (
            numPlayers_     // acting player idx
            + numMoveTypes_ // move type
            + numPlayers_   // target player idx
            + numColors_    // color revealed
            + numRanks_     // rank revealed
            + handSize_     //  reveal outcome, each bit is 1 if card was hinted at
            + handSize_     // postion played/discarded
            + bitsPerCard_  // card played/discarded
            + 2);           // successful, added info token

    const int beliefSectionLen_ = (
            numPlayers_ * handSize_ * (numColors_ * numRanks_ + numColors_ + numRanks_));

    std::vector<float> handSection_;
    std::vector<float> boardSection_;
    std::vector<float> discardSection_;
    std::vector<float> lastActionSection_;
    std::vector<float> beliefSection_;

public:
    HleSerializedMove(State s,
                      move lastMove,
                      Card lastCard,  // for play/discard
                      Card lastMoveIndices,  // for hint color/rank
                      int prevScore,
                      int prevNumHint,
                      const std::vector<FactorizedBeliefs> &v0Beliefs)
            : numPlayers_(s.get_num_players())
            , handSize_(1) // TODO: need to replace with HandSizeFromRules(numPlayers_) but for HanaSim

    {
        handSection_ = encodeHands(s);
        boardSection_ = encodeBoard(s);
        discardSection_ = encodeDiscard(s);
        lastActionSection_ = encodeLastAction(
                s, lastMove, lastCard, lastMoveIndices, prevScore, prevNumHint);
        beliefSection_ = encodeBelief(s, v0Beliefs);

        // std::cout << "hand section: " << handSection_.size() <<std::endl;
        // std::cout << "board section: " << boardSection_.size() <<std::endl;
        // std::cout << "discard section: " << discardSection_.size() <<std::endl;
        // std::cout << "lastAction section: " << lastActionSection_.size() <<std::endl;
        // std::cout << "belief section: " << beliefSection_.size() <<std::endl;
    }

    int numMoves() const {
        // final +1 for noOp
        return 2 * handSize_ + (numPlayers_ - 1) * numColors_ + (numPlayers_ - 1) * numRanks_ + 1;
    }

    int numPlayers() const {
        return numPlayers_;
    }

    int handSize() const {
        return handSize_;
    }

    int numColors() const {
        return numColors_;
    }

    int numRanks() const {
        return numRanks_;
    }

    std::vector<float> encodeHands(State s) {
    }

    std::vector<float> encodeBoard(State s) {

    }

    std::vector<float> encodeDiscard(State s) {

    }

    std::vector<float> encodeLastAction(State s,
            move lastMove,
            Card lastCard,               // for play/discard
            Card lastMoveIndices, // for hint color/rank
            int prevScore,
            int prevNumHint) {
    }

    std::vector<float> encodeBelief( State s, const std::vector<FactorizedBeliefs>& v0Beliefs) {
    }

    void dumpArray() const {
        assert(false);
        // printf("SerializedMove: ");
        // const float *data = reinterpret_cast<const float *>(this);
        // for (int i = 0; i < sizeof(*this) / sizeof(float); i++) {
        //   printf("%.1f ", data[i]);
        // }
        // printf("\n");
    }

    void write(std::ostream &out) const {
        assert(false);
        // out.write(reinterpret_cast<const char*>(this), sizeof(*this));
        // out.flush();
    }

    void log(std::ostream &out) const {
        auto s = toArray();
        std::cout << "size of feature: " << s.size() << std::endl;
        out << "hands" << std::endl;
        for (auto v : handSection_) {
            out << v << " ";
        }
        out << std::endl;

        out << "board" << std::endl;
        for (auto v : boardSection_) {
            out << v << " ";
        }
        out << std::endl;

        out << "discard" << std::endl;
        for (auto v : discardSection_) {
            out << v << " ";
        }
        out << std::endl;

        out << "last action" << std::endl;
        for (auto v : lastActionSection_) {
            out << v << " ";
        }
        out << std::endl;

        out << "card knowledge" << std::endl;
        for (auto v : beliefSection_) {
            out << v << " ";
        }
        out << std::endl;
    }

    float sum() const {
        auto v = toArray();
        float s = 0;
        for (auto val : v) {
            s += val;
        }
        return s;
    }

    std::vector<float> toArray() const {
        std::vector<float> res;
        res.insert(res.end(), handSection_.begin(), handSection_.end());
        res.insert(res.end(), boardSection_.begin(), boardSection_.end());
        res.insert(res.end(), discardSection_.begin(), discardSection_.end());
        res.insert(res.end(), lastActionSection_.begin(), lastActionSection_.end());
        res.insert(res.end(), beliefSection_.begin(), beliefSection_.end());
        if (HleParams::GREEDY_ACTION) {
            res.insert(res.end(), lastActionSection_.begin(), lastActionSection_.end());
        }
        // std::cout << "size of feature: " << res.size() << std::endl;
        return res;
    }
};

int move_to_index(move m, State s, int id, int num_cards, int num_players) {
    int NUM_COLORS = 5;
    int NUM_RANKS = 5;
    int max_reveal_color = (num_players - 1) * NUM_COLORS;
    int targ_offset = (m.get_to() + num_players - id) % num_players;
    if (m.get_type() == DISCARD) {
        return m.get_card_index();
    } else if (m.get_type() == PLAY) {
        return m.get_card_index() + num_cards;
    } else if (m.get_type() == COL_HINT) {
        return ((targ_offset - 1) * NUM_COLORS) + (m.get_color() - 1) + (2 * num_cards);
    } else if (m.get_type() == RANK_HINT) {
        return ((targ_offset - 1) * NUM_RANKS) + (m.get_rank() - 1) + (2 * num_cards) + max_reveal_color;
    }
    return -1;
}

