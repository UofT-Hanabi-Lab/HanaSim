#pragma once

#include "../../include/belief_state.h"
#include "../../include/move.h"
#include "smartbot.h"
#include "../../include/state.h"
#include <memory>


class spartabot final: public player {
public:
    spartabot(int id, int n_players, int search_n = 100, float search_thresh = 0.5);

    void initialize_belief(const State& s);

    void observe(State s, move m);
    void observe_before_move(State s);
    move play(State s) override;
    move play_prechosen(State s, move m);
    int get_id();
    int get_n_cards();

private:
    std::shared_ptr<smartbot> bp_;  // the blueprint bot (SmartBot)
    int id_;
    int n_players_;
    int search_n_;// number of simulations
    float search_thresh_; // use the new move only if newmove improves score by more than the threshold

    BeliefState bs_;
    float simulate_game(State s);
};
