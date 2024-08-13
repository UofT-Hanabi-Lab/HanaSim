#include "../../include/player.h"


#pragma once

class randombot : public player {
    int num_cards_;
    int id_;


public:
    randombot(int id, int num_players);
    std::vector<move> get_legal_moves(State s, int id);
    move play(State s);
    void observe(State s, move m);
    void observe_before_move(State s);
    int get_id();
    int get_n_cards();
};