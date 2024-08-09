#pragma once
#include "../../include/player.h"
#include "../../include/move.h"
#include "threadpool.h"
#include <iostream>
#include <set>


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
    
    void updateFromHint(move m, State s) {
        handSize = s.get_hands()[p_].size();
        for (int j = 0; j < 25; j++) {
            Card c = index_to_card(j);
            int card_value = m.get_type() == COL_HINT ? ((int) c.color()) : ((int) c.rank());
            bool matches = m.get_type() == COL_HINT ? ((int) m.get_color() == c.color()) : ((int) m.get_rank() == c.rank());
            for (int i = 0; i < handSize; i ++) {
                bool found = false;
                for (int l = 0; l < m.get_card_indices().size(); l++) {
                    if (m.get_card_indices()[l] == i) {
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
                for (int l = 0; l < m.get_card_indices().size(); l++) {
                    if (m.get_card_indices()[l] == i) {
                        found = true; 
                        break;
                    }
                }
                if (found) colorRevealed.set(i * 5 + ((int) m.get_color() - 1), 1);
            }
        } else {
            for (int i = 0; i < handSize; i ++) {
                bool found = false;
                for (int l = 0; l < m.get_card_indices().size(); l++) {
                    if (m.get_card_indices()[l] == i) {
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
    int id_;

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
                      int id,
                      move lastMove,
                      Card lastCard,  // for play/discard
                      std::vector<int> lastMoveIndices,  // for hint color/rank
                      int prevScore,
                      int prevNumHint,
                      const std::vector<FactorizedBeliefs> &v0Beliefs)
            : numPlayers_(s.get_num_players())
            , id_(id)
            , handSize_(5) // TODO: need to replace with HandSizeFromRules(numPlayers_) but for HanaSim

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
        std::vector<float> handSection(handSectionLen_, 0);

        // self-hand is all zero
        int codeOffset = bitsPerHand_;
        for (int playerOffset = 1; playerOffset < numPlayers_; ++playerOffset) {
            const int pidx = (playerOffset + id_) % numPlayers_;
            std::vector<Card> hand = s.get_hands()[pidx];
            for (int cidx = 0; cidx < (int)hand.size(); ++cidx) {
                int index = card_to_index(hand[cidx]);
                assert(index < bitsPerCard_);
                handSection[codeOffset + index] = 1;
                codeOffset += bitsPerCard_;
            }
            codeOffset += bitsPerCard_ * (handSize_ - (int)hand.size());
        }

        for (int playerOffset = 0; playerOffset < numPlayers_; ++playerOffset) {
            const int pidx = (playerOffset + id_) % numPlayers_;
            int size = s.get_hands()[pidx].size();
            assert(size <= handSize_);
            if (size < handSize_) {
                handSection[codeOffset] = 1;
            }
            codeOffset += 1;
        }
        return handSection;
    }

    std::vector<float> encodeBoard(State s) {
        std::vector<float> boardSection(boardSectionLen_, 0);
        int codeOffset = 0;

        int remainingDeck = s.get_deck().size();
        std::fill(boardSection.begin(), boardSection.begin() + remainingDeck, 1);
        codeOffset += maxRemainingDeckSize_;

        for (int i = 0; i < numColors_; ++i) {
            int pile_top = s.get_piles()[i + 1];
            if (pile_top == 0) {
                codeOffset += numRanks_;
                continue;
            }
            int index = pile_top - 1;
            boardSection[codeOffset + index] = 1;
            codeOffset += numRanks_;
        }

        int remainingInfo = s.get_num_hints();
        auto infoStart = boardSection.begin() + codeOffset;
        std::fill(infoStart, infoStart + remainingInfo, 1);
        codeOffset += maxNumInfoTokens_;

        int remainingLife = s.get_num_lives();
        auto lifeStart = boardSection.begin() + codeOffset;
        std::fill(lifeStart, lifeStart + remainingLife, 1);
        return boardSection;
    }

    std::vector<float> encodeDiscard(State s) {
        std::vector<float> discardSection(maxDeckSize_, 0);

        std::vector<int> discardCount(numColors_ * numRanks_, 0);
        for (Card c : s.get_discards()) {
            ++discardCount[card_to_index(c)];
        }

        int offset = 0;
        for (int c = 0; c < numColors_; ++c) {
            for (int r = 0; r < numRanks_; ++r) {
                int numDiscarded = discardCount[c * numRanks_ + r];
                int numCard = NumberCardInstance(c, r);
                for (int i = 0; i < numCard; ++i) {
                    discardSection[offset + i] = i < numDiscarded ? 1 : 0;
                }
                offset += numCard;
            }
        }
        return discardSection;
    }

    std::vector<float> encodeLastAction(State s,
            move lastMove,
            Card lastCard,               // for play/discard
            std::vector<int> lastMoveIndices, // for hint color/rank
            int prevScore,
            int prevNumHint) {
        std::vector<float> lastAction(lastActionSectionLen_, 0);
        // first step, no prev action
        if (lastMove.get_type() == INVALID_MOVE) {
            return lastAction;
        }

        int offset = 0;
        int lastActivePlayer = (s.get_active_player() + numPlayers_ - 1) % numPlayers_;
        int relativeIdx = (numPlayers_ + lastActivePlayer - id_) % numPlayers_;
        lastAction[relativeIdx] = 1;
        offset += numPlayers_;

        // move type
        int typeIdx = (int)lastMove.get_type() - 1;
        lastAction[offset + typeIdx] = 1;
        offset += numMoveTypes_;

        if (lastMove.get_type() == COL_HINT || lastMove.get_type() == RANK_HINT) {
            int targetPlayer = (lastMove.get_to() + numPlayers_ - id_) % numPlayers_;
            lastAction[offset + targetPlayer] = 1;
        }
        offset += numPlayers_;

        // color revealed
        if (lastMove.get_type() == COL_HINT) {
            lastAction[offset + ((int)lastMove.get_color() - 1)] = 1;
        }
        offset += numColors_;

        // value revealed
        if (lastMove.get_type() == RANK_HINT) {
            lastAction[offset + ((int)lastMove.get_rank() - 1)] = 1;
        }
        offset += numRanks_;

        // reveal outcome
        if (lastMove.get_type() == RANK_HINT || lastMove.get_type() == COL_HINT) {
            for (int i = 0; i < handSize_; i++) {
                for (int j = 0; j < lastMoveIndices.size(); j++) {
                    if (lastMoveIndices[j] == i) lastAction[offset + i] = 1; break; 
                }
            }
        }
        offset += handSize_;

        // position played/discarded
        if (lastMove.get_type() == PLAY || lastMove.get_type() == DISCARD) {
            lastAction[offset + lastMove.get_card_index()] = 1;
        }
        offset += handSize_;

        // card played/discarded
        if (lastMove.get_type() == PLAY || lastMove.get_type() == DISCARD) {
            lastAction[offset + card_to_index(lastCard)] = 1;
        }
        offset += bitsPerCard_;

        // std::cout << offset << ", " << LAST_ACTION_SIZE << std::endl;
        // whether success play
        if (lastMove.get_type() == PLAY) {
            int curr_score = 0;
            for (int top : s.get_piles()) curr_score += top;
            if (curr_score > prevScore) {
                lastAction[offset] = 1;
            }
            if (s.get_num_hints() > prevNumHint) {
                lastAction[offset + 1] = 1;
            }
        }
        offset += 2;
        return lastAction;
    }

    std::vector<float> encodeBelief( State s, const std::vector<FactorizedBeliefs>& v0Beliefs) {
        std::vector<float> beliefSection(beliefSectionLen_, 0);

        int offset = 0;
        for (int playerOffset = 0; playerOffset < numPlayers_; ++playerOffset) {
            int player = (playerOffset + id_) % numPlayers_;
            const auto& belief = v0Beliefs[player];
            auto beliefArray = belief.get();
            for (int cardIdx = 0; cardIdx < handSize_; ++cardIdx) {
                for (int i = 0; i < bitsPerCard_; ++i) {
                    beliefSection[offset + i] = beliefArray[cardIdx][i];
                }
                offset += bitsPerCard_;

                for (int i = 0; i < numColors_; ++i) {
                    beliefSection[offset] = belief.colorRevealed.get(cardIdx * 5 + i);
                    ++offset;
                }

                for (int i = 0; i < numRanks_; ++i) {
                    beliefSection[offset] = belief.rankRevealed.get(cardIdx * 5 + i);
                    ++offset;
                }
            }
        }
        return beliefSection;
    }
    std::vector<float> toArray() {
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

std::vector<move> get_legal_moves(State s, int id) {
    std::vector<move> moves;
    moves = {};
    std::vector<std::vector<Card>> hands = s.get_hands();
    if (s.get_num_hints() > 0) {
        for (int i = 0; i < hands.size(); i++) {
            std::set<Color> colors;
            std::set<Rank> ranks;
            if (i == id) {
                continue;
            }
            for (int j = 0; j < hands[i].size(); j++) {
                Card c = hands[i][j];
                colors.insert(c.color());
                ranks.insert(c.rank());
            }
            for (Color col : colors) {
                std::vector<int> indices;
                for (int j = 0; j < hands[i].size(); j++) {
                    if (hands[i][j].color() == col) indices.push_back(j);
                }
                moves.push_back(move(COL_HINT, i, id, indices, col));
            };
            for (Rank rank : ranks) {
                std::vector<int> indices;
                for (int j = 0; j < hands[i].size(); j++) {
                    if (hands[i][j].rank() == rank) indices.push_back(j);
                }
                moves.push_back(move(RANK_HINT, i, id, indices, rank));
            }
        }
    }
    if (s.get_num_hints() < 8) {
        for (int i = 0; i < hands[id].size(); i++) {
            moves.push_back(move(DISCARD, id, i));
        }
    }
    for (int i = 0; i < hands[id].size(); i++) {
        moves.push_back(move(PLAY, id, i));
    }
    return moves;
}

inline ThreadPool &getThreadPool() {
    static std::shared_ptr<ThreadPool> pool;
    if (!pool || pool->stop) {
        pool.reset(new ThreadPool(10)); // HARD-CODED 10 threads
    }
    return *pool;
}