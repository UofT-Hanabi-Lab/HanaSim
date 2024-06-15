/*


A class for a Hanabi player.


*/
#include "state.h"


#pragma once

class player
{
    int num_cards_;
    int id_; // should be unique and in order of 0,1,2,3,...


public:
    player() {};
    virtual move play(State s) = 0; // abstract method
    //virtual move play(State s, int curr_player) = 0; // abstract method
    virtual void observe(move m) = 0; // abstract method
    int get_id();
    int get_n_cards();
};
