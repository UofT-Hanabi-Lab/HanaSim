#pragma once
#include "../../include/state.h"
#include "../../include/move.h"

namespace HleParams {
    // read in parameters from environment variable
//    const int GREEDY_ACTION = Params::getParameterInt("GREEDY_ACTION", 1);
    const int GREEDY_ACTION = 1; // TODO replaced line above
} // namespace HleParams

typedef std::map<Card, int> DeckComposition;
void addToDeck(const std::vector<Card> &cards, DeckComposition &deck);
void removeFromDeck(const std::vector<Card> &cards, DeckComposition &deck);
DeckComposition getCurrentDeckComposition(const State s, int who);


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

struct FactorizedBeliefs {
    FactorizedBeliefs(State s, int player);
    std::array<std::array<float, 25>, 5> get() const;
    void log();
    void updateFromHint(move m, Card c_indices, State s);
    void updateFromRevealedCard(Card played_card, const DeckComposition &deck, State s);
    void updateFromDraw(const DeckComposition &deck, int card_index, State s);

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