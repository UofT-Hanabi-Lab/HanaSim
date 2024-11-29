#include "player.h"


#pragma once

class humanplayer : public player {
    int num_cards_;
    int id_;


public:
    humanplayer(int num_players, int id);
    move play(State s);
    move play_prechosen(State s, move m);
    void observe(State s, move m);
    void observe_before_move(State s);

    // Getters
    int get_id();
    int get_n_cards();

    void print_move(move m, int index);
    void print_hands(State s);
};