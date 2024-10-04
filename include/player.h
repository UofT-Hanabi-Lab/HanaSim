/*


A class for a Hanabi player.


*/
#include "state.h"
#include <map>


#pragma once

class player
{
    int num_cards_;
    int id_; // should be unique and in order of 0,1,2,3,...


public:
    player() {};
    virtual move play(State s) = 0; // abstract method
    virtual move play_prechosen(State s, move chosen_m) = 0; // for unit test
    //virtual std::vector<move> get_legal_moves(State s, int id); // for move checker
    std::vector<move> get_legal_moves(State s, int id); // for move checker
    //virtual move play(State s, int curr_player) = 0; // abstract method
    virtual void observe(State s, move m) = 0; // abstract method
    virtual void observe_before_move(State s) = 0; // abstract
    int get_id(); // changed to virtual 
    int get_n_cards();
};
