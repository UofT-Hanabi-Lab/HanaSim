#pragma once

#include "state.h"
#include "move.h"
#include "smartbot.h"
#include <memory>

class SearchSmartBot {
public:
    SearchSmartBot(int id, int n_players, int search_n = 100, float search_thresh = 0.5);

    void observe(State s, move m);
    move play(State s);
    move play_prechosen(State s, move m);
    int get_id();
    int get_n_cards();

private:
    std::shared_ptr<smartbot> bp_;  // the blueprint bot (SmartBot)
    int id_;
    int n_players_;
    int search_n_;
    float search_thresh_;

    float simulate_game(State s);
};
