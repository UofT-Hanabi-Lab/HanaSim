#include <iostream>
#include <stdio.h>
#include <numeric>

#include "../include/game.h"


State game::get_curr_state() {
    return curr_state_;
}

game::game(State init_state, std::vector<player*> players) : curr_state_(init_state), players_(players){
}


int game::run(bool log_game) {
    int curr_score = 0;
    if (log_game) std::cout << "STARTING GAME\n" << std::endl;
    int curr_player = 0;
    int turn = 1;
    while ((curr_score < 25) && (curr_state_.get_num_lives() > 0) && !(curr_state_.get_deck().empty())) {
        if (log_game) std::cout << "TURN " << turn << " PLAYER " << curr_player << std::endl;
        move next_move = (*(players_[curr_player])).play(curr_state_);
        if (log_game) next_move.str();


        for (int i = 0; i < players_.size(); i++) {
            State s = curr_state_;
            (*(players_[i])).observe(curr_state_, next_move);
        }
        
        curr_state_.transition(next_move, log_game);
        int score = 0;
        for (int top : curr_state_.get_piles()) {
            score += top;
        }
        curr_score = score;
        turn++;
        curr_player = (curr_player + 1) % players_.size();
    }
    if (curr_state_.get_num_lives() == 0) {
        if (log_game) std::cout << "GAME OVER. SCORE:" << curr_score << std::endl;
        return curr_score;
    } else if (curr_state_.get_deck().empty()) {
        for (int i = 0; i < players_.size(); i++) {
            if (log_game) std::cout << "TURN " << turn << " PLAYER " << curr_player << std::endl;
            move next_move = (*(players_[curr_player])).play(curr_state_);
            if (log_game) next_move.str();
            for (int i = 0; i < players_.size(); i++) {
                (*(players_[i])).observe(curr_state_, next_move);
            }
            curr_state_.transition(next_move, log_game);
            int score = 0;
            for (int top : curr_state_.get_piles()) {
                score += top;
            }
            curr_score = score;
            turn++;
            curr_player = (curr_player + 1) % players_.size();
        }
        if (curr_state_.get_num_lives() == 0) {
            if (log_game) std::cout << "GAME OVER. SCORE:" << curr_score << std::endl;
            return curr_score;
        } else if (curr_score == 25) {
            if (log_game) std::cout << "GAME WON!" << std::endl;
            return curr_score;
        } else {
            if (log_game) std::cout << "GAME FINISHED. SCORE:" << curr_score << std::endl;
            return curr_score;
        }
    } else {
        if (log_game) std::cout << "GAME WON!" << std::endl;
        return curr_score;
    }
}