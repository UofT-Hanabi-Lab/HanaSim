#include <iostream>
#include <stdio.h>
#include <numeric>

#include "../include/game.h"
#include "../include/humanplayer.h"


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
        for (int i = 0; i < players_.size(); i++) {
            State s = curr_state_;
            (*(players_[i])).observe_before_move(curr_state_);
        }


        std::cout << "1234556\n" << std::endl;
        move next_move = (*(players_[curr_player])).play(curr_state_); // holmesbot code breaks here
        if (log_game) next_move.str(); 
        std::cout << "1234556\n" << std::endl;


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
            for (int i = 0; i < players_.size(); i++) {
                State s = curr_state_;
                (*(players_[i])).observe_before_move(curr_state_);
            }
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
            if (curr_state_.get_num_lives() == 0) {
                if (log_game) std::cout << "GAME OVER. SCORE:" << curr_score << std::endl;
                return curr_score;
            } else if (curr_score == 25) {
                if (log_game) std::cout << "GAME WON!" << std::endl;
                return curr_score;
            }
        }
        if (log_game) std::cout << "GAME FINISHED. SCORE:" << curr_score << std::endl;
        return curr_score;
    } else {
        if (log_game) std::cout << "GAME WON!" << std::endl;
        return curr_score;
    }
}

int game::run_test(bool log_game, std::vector<move> test_moves) {
    int curr_score = 0;
    if (log_game) std::cout << "STARTING TEST GAME\n" << std::endl;
    int curr_player = 0;
    int turn = 1;
    while ((curr_score < 25) && (curr_state_.get_num_lives() > 0) && !(curr_state_.get_deck().empty())) {
        if (log_game) std::cout << "TURN " << turn << " PLAYER " << curr_player << std::endl;
        for (int i = 0; i < players_.size(); i++) {
            State s = curr_state_;
            (*(players_[i])).observe_before_move(curr_state_);
        }

        move chosen = test_moves.back();
        test_moves.pop_back();
        //std::vector<move> legal_moves = (players_[curr_player])->get_legal_moves(curr_state_, curr_player);
                std::cout << "hello "<< std::endl;
        move next_move = (*(players_[curr_player])).play_prechosen(curr_state_, chosen); //here
                std::cout << "hello123 "<< std::endl;
        //next_move = chosen; 


        // int c = count(legal_moves.begin(), legal_moves.end(), chosen);
        // std::cout << c << "helo" << std::endl;
        // if (c== 1){
        //     throw std::invalid_argument( "received invalid move" );
        // }


        if (log_game) next_move.str();

std::cout << "broken?? "<< std::endl;
        for (int i = 0; i < players_.size(); i++) {
            State s = curr_state_;
            std::cout << "broken??123 "<< std::endl;
            (*(players_[i])).observe(curr_state_, next_move); //broken here
            std::cout << "broken??312 "<< std::endl;
        }
        std::cout << "broken? "<< std::endl;

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
            for (int i = 0; i < players_.size(); i++) {
                State s = curr_state_;

                (*(players_[i])).observe_before_move(curr_state_);
            }
            
            move chosen = test_moves.back();
            test_moves.pop_back();
            // std::vector<move> legal_moves = (players_[curr_player])->get_legal_moves(curr_state_, curr_id);
            move next_move = (*(players_[curr_player])).play_prechosen(curr_state_, chosen); //here
            //next_move = chosen; 
           
           
            // int c = count(legal_moves.begin(), legal_moves.end(), chosen);
            // std::cout << c << "helo" << std::endl;
            // if (c==1){
            //     throw std::invalid_argument( "received invalid move" );
            // }

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
            if (curr_state_.get_num_lives() == 0) {
                if (log_game) std::cout << "GAME OVER. SCORE:" << curr_score << std::endl;
                return curr_score;
            } else if (curr_score == 25) {
                if (log_game) std::cout << "GAME WON!" << std::endl;
                return curr_score;
            }
        }
        if (log_game) std::cout << "GAME FINISHED. SCORE:" << curr_score << std::endl;
        return curr_score;
    } else {
        if (log_game) std::cout << "GAME WON!" << std::endl;
        return curr_score;
    }
}