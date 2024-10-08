#include "player.h"


#pragma once

class humanplayer : public player {
    int num_cards_;
    int id_;


public:
    humanplayer(int num_players, int id);
    std::vector<move> get_legal_moves(State s, int id);
    move play(State s);
    void observe(State s, move m);
    void observe_before_move(State s);
    int get_id();
    int get_n_cards();

    void print_move(move m);

    void print_move(move m, int index);

    void print_hands(State s);

//    move play(State s, int curr_player);
};