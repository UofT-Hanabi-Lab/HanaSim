#include "card.h"
#include "move.h"
#include <vector>


class State {

    int num_players_;
    int cards_per_hand_;
    std::vector<std::vector<Card>> hands_;
    int hint_tokens_;
    int lives_;
    std::vector<Card> deck_;
    std::vector<Card> init_deck_;
    std::vector<Card> discards_;
    std::vector<int> piles_; // rank of top most card in each pile (0 if pile is empty)

public:
    State(int num_players);
    State(int num_players, std::vector<Card> deck); // provide an already shuffled deck

    std::vector<std::vector<Card>> get_hands() const;
    int get_num_hints() const;
    int get_num_lives() const;
    std::vector<Card> get_discards() const;
    std::vector<int> get_piles() const;
    std::vector<Card> get_deck() const;

    void transition(move m, bool log);
    void reset();
};
