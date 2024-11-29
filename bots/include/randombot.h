#include "../../include/player.h"


#pragma once

class randombot : public player {
    int num_cards_;
    int id_;


public:
    randombot(int id, int num_players);
    move play(State s);
    move play_prechosen(State s, move chosen_m);
    void observe(State s, move m);
    void observe_before_move(State s);
    int get_id();
    int get_n_cards();
};