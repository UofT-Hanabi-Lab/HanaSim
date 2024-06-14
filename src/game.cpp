#include <iostream>
#include <stdio.h>
#include <numeric>

#include "../include/game.h"


State game::get_curr_state() {
    return curr_state_;
}

game::game(State init_state, std::vector<player*> players) : curr_state_(init_state), players_(players) {
}

void game::run() {
    int curr_score = 0;
    std::cout << "STARTING GAME\n" << std::endl;
    int curr_player = 0;
    int turn = 1;
    while ((curr_score < 25) && (curr_state_.get_num_lives() > 0) && !(curr_state_.get_deck().empty())) {
        std::cout << "TURN " << turn << " PLAYER " << curr_player << std::endl;
        move next_move = (*(players_[curr_player])).play(curr_state_);
        next_move.str();
        std::cout << "\n";
        curr_state_.transition(next_move);
        for (int i = 0; i < players_.size(); i++) {
            (*(players_[i])).observe(next_move);
        }
        int score = 0;
        for (int top : curr_state_.get_piles()) {
            score += top;
        }
        curr_score = score;
        turn++;
        curr_player = (curr_player + 1) % players_.size();
    }
    if (curr_state_.get_num_lives() == 0) {
        std::cout << "GAME OVER. SCORE:" << curr_score << std::endl;
    } else if (curr_state_.get_deck().empty()) {
        for (int i = 0; i < players_.size(); i++) {
            move next_move = (*(players_[curr_player])).play(curr_state_);
            next_move.str();
            std::cout << "\n";
            curr_state_.transition(next_move);
            for (int i = 0; i < players_.size(); i++) {
                (*(players_[i])).observe(next_move);
            }
            curr_score = accumulate(curr_state_.get_piles().begin(), curr_state_.get_piles().end(), 0);
        }
        if (curr_state_.get_num_lives() == 0) {
            std::cout << "GAME OVER. SCORE:" << curr_score << std::endl;
        } else if (curr_score == 25) {
            std::cout << "GAME WON!" << std::endl;
        } else {
            std::cout << "GAME FINISHED. SCORE:" << curr_score << std::endl;
        }
    } else {
        std::cout << "GAME WON!" << std::endl;
    }
}