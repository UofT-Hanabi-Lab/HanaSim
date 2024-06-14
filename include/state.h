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
    std::vector<Card> discards_;
    std::vector<int> piles_; // rank of top most card in each pile (0 if pile is empty)

public:
    State(int num_players, int cards_per_hand);

    std::vector<std::vector<Card>> get_hands();
    int get_num_hints();
    int get_num_lives();
    std::vector<Card> get_discards();
    std::vector<int> get_piles();
    std::vector<Card> get_deck();

    void transition(move m);
};
