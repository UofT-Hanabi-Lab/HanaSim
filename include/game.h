#include "player.h"
#pragma once

class game {
    std::vector<player*> players_;
    State curr_state_;

public:
    game(State init_state, std::vector<player*>);
    int run(bool log_game);
    //int create_hand_knowledge(std::vector<player*> players, int curr_player);
    int write_move_csv(std::ofstream &file, move move);
    int write_hand_csv(std::ofstream &file, State s, int curr_p, int num_p);
    int write_pile_csv(std::ofstream &file, State s);
    int write_deck_and_discards_csv(std::ofstream &file, State s);
    int combine_csv(std::ofstream &out, std::string in1, std::string in2, std::string in3, std::string in4, int turn);
    int tree_csv(std::ofstream &out, std::ofstream &out2, std::string in1, std::string in2, std::string in3, std::string in4, 
        std::string in5,int turn);
    State get_curr_state();

    int run_test(bool log_game, std::vector<move> test_moves);
};

