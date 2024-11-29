#include <random>
#include <set>
#include <algorithm>
#include <iterator>
#include <iostream>


#include "include/randombot.h"




randombot::randombot(int id, int num_players) : num_cards_((num_players <= 3) ? 5 : 4), id_(id) {    
}


void randombot::observe(State s, move m) {};

void randombot::observe_before_move(State s) {};


int randombot::get_id() {
    return id_;
}


int randombot::get_n_cards() {
    return num_cards_;
}

// just pick a random move from legal moves
move randombot::play(State s) {
    std::vector<move> legal_moves = s.get_legal_moves(id_);
    std::vector<move> m;
    std::sample(legal_moves.begin(), legal_moves.end(), std::back_inserter(m), 1, std::mt19937{std::random_device{}()});
    return m[0];
}

// Not implemented
move randombot::play_prechosen(State s, move chosen_m) {
    return move(INVALID_MOVE);
}
