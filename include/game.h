#include "player.h"
#pragma once

class game {
    std::vector<player*> players_;
    State curr_state_;

public:
    game(State init_state, std::vector<player*>);
    int run(bool log_game);
    State get_curr_state();

    int run_test(bool log_game, std::vector<move> test_moves);
};

