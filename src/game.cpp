#include <iostream>
#include <stdio.h>
#include <numeric>
#include <string>
#include <fstream>
#include <string>
#include <direct.h> // for _mkdir

#include "../include/game.h"
#include "../include/humanplayer.h"


State game::get_curr_state() {
    return curr_state_;
}

game::game(State init_state, std::vector<player*> players) : curr_state_(init_state), players_(players){
}

int game::write_move_csv(std::ofstream &moveFile, move next_move){
    moveFile << "[";
    if(next_move.get_type() == 1){
        moveFile << next_move.get_type() << next_move.get_to() << next_move.get_from();
        moveFile << "[";
        std::vector<int> indices = next_move.get_card_indices();
        for(int x = 0; x < indices.size(); x++){
            moveFile << indices[x];
            if(x != indices.size() - 1) moveFile << "; ";
        }
        moveFile << "]" << next_move.get_color();  
    }
    if(next_move.get_type() == 2){
        moveFile << next_move.get_type() << next_move.get_to() << next_move.get_from();
        moveFile << "[";
        std::vector<int> indices = next_move.get_card_indices();
        for(int x = 0; x < indices.size(); x++){
            moveFile << indices[x];
            if(x != indices.size() - 1) moveFile << "; ";
        }
        moveFile << "]" << next_move.get_rank();    
    }
    if(next_move.get_type() == 3 || next_move.get_type() == 4){
        moveFile << next_move.get_type() << next_move.get_from() << next_move.get_card_index();  
    }
    moveFile << "]" << std::endl;
    return 0; 
}

int game::write_hand_csv(std::ofstream &handFile, State s, int curr_p, int num_p){
    handFile << "[";
    for (int x = 0; x < num_p; x++) {
        // for writing to csv
        std::vector<std::vector<Card>> hands = s.get_hands();
        std::cout << "player " << x << std::endl;
        for (int y=0; y < hands[x].size(); y++){
            if(x == curr_p){
                    hands[x][y] = Card(invalid_color, invalid_rank);
            }
            std::cout << hands[x][y].color() << hands[x][y].rank() << std::endl;
            handFile << hands[x][y].color() << hands[x][y].rank();
            if(y != hands[x].size() - 1) handFile << "; ";
        }
        if(x != hands.size() - 1) handFile << "; ";
    }
    handFile << "]" << std::endl;
    return 0;
}

int game::write_pile_csv(std::ofstream &pileFile, State s){
    std::vector<int> piles = curr_state_.get_piles();
    pileFile << "[";
    // start at index 1, apparently index 0 is for invalid card pile?
    for (int x = 1; x < piles.size(); x++){
        pileFile << piles[x];
    }
    pileFile << "]" << std::endl;
    return 0;
}

int game::write_deck_csv(std::ofstream &deckFile, State s){
    std::vector<Card> deck = s.get_deck();
    deckFile << "[";
    for (int x = 0; x < deck.size(); x++){
        deckFile << deck[x].color() << deck[x].rank();
        if(x != deck.size() - 1) deckFile << "; ";
    }
    deckFile << "]" << std::endl;
    return 0;
}

int game::combine_csv(std::ofstream &outFile, std::string dfile, std::string pfile, std::string hfile, std::string mfile, int turn){
    std::ifstream handFile(hfile);
    std::ifstream moveFile(mfile);
    std::ifstream pileFile(pfile);
    std::ifstream deckFile(dfile);

    std::string deck;
    std::string pile;
    std::string hand;
    std::string move;

    for(int x = 1; x < turn; x++){
        outFile << x << ": observations" << ",";
    }
    
    outFile << ". ,";
    for(int x = 1; x < turn; x++){
        outFile << x << ": move" << ",";
    }
    outFile << std::endl;

    while(std::getline(deckFile, deck)) {
        std::getline(pileFile, pile);
        std::getline(handFile, hand);
        // std::cout << hand << std::endl;
        // std::cout << pile << std::endl;
        outFile << "[" << deck << "; " << pile << "; " << hand << "],";
    }
    outFile << ". ,";
    while(std::getline(moveFile, move)) {
        outFile << move << ",";
    }
    handFile.close();
    deckFile.close();
    pileFile.close();
    moveFile.close();
    return 0;
}

int game::tree_csv(std::ofstream &outFile, std::ofstream &outFile2, std::string dfile, std::string pfile, std::string hfile, std::string mfile, int turn){
    //assume only 2 player
    std::ifstream handFile(hfile);
    std::ifstream moveFile(mfile);
    std::ifstream pileFile(pfile);
    std::ifstream deckFile(dfile);

    std::string deck;
    std::string pile;
    std::string hand;
    std::string move;

    std::vector<std::string> attributes = {"hand", "deck", "pile", "move"}; 

    for(int i =0; i < attributes.size(); i++){
        outFile << attributes[i];
        if(i != attributes.size() - 1) outFile << ", ";

        outFile2 << attributes[i];
        if(i != attributes.size() - 1) outFile2 << ", ";
    }

    outFile << std::endl;
    outFile2 << std::endl;

    for (int x = 1; x < turn; x++) {
        std::getline(deckFile, deck);
        std::getline(pileFile, pile);
        std::getline(handFile, hand);
        std::getline(moveFile, move);
        if (x % 2 == 1){
            outFile << hand << ", " << deck << ", " << pile << ", " << move << std::endl; 
        }
        else{
            outFile2 << hand << ", " << deck << ", " << pile << ", " << move << std::endl; 
        }
        // std::cout << hand << std::endl;
        // std::cout << pile << std::endl;
    }
    handFile.close();
    deckFile.close();
    pileFile.close();
    moveFile.close();
    return 0;
}


int game::run(bool log_game) {
    
    int curr_score = 0;
    if (log_game) std::cout << "STARTING GAME\n" << std::endl;
    int curr_player = 0;
    int turn = 1;

    std::ofstream handFile("output/hands.csv");
    std::ofstream moveFile("output/moves.csv");
    std::ofstream pileFile("output/piles.csv");
    std::ofstream deckFile("output/decks.csv");
    std::ofstream outFile("output/p1.csv");
    std::ofstream outFile2("output/p2.csv");

    
    while ((curr_score < 25) && (curr_state_.get_num_lives() > 0) && !(curr_state_.get_deck().empty())) {
        if (log_game) std::cout << "TURN " << turn << " PLAYER " << curr_player << std::endl;
        
        // write csv
        for (int i = 0; i < players_.size(); i++) {
            State s = curr_state_;
            (*(players_[i])).observe_before_move(curr_state_);
        }
        write_hand_csv(handFile, curr_state_, curr_player, players_.size());
        write_pile_csv(pileFile, curr_state_);
        write_deck_csv(deckFile, curr_state_);


        move next_move = (*(players_[curr_player])).play(curr_state_);
        //write move csv
        write_move_csv(moveFile, next_move);
        
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
        //return curr_score;
    } else if (curr_state_.get_deck().empty()) {
        for (int i = 0; i < players_.size(); i++) {
            write_hand_csv(handFile, curr_state_, curr_player, players_.size());
            write_pile_csv(pileFile, curr_state_);
            write_deck_csv(deckFile, curr_state_);
            if (log_game) std::cout << "TURN " << turn << " PLAYER " << curr_player << std::endl;
            for (int i = 0; i < players_.size(); i++) {
                State s = curr_state_;
                (*(players_[i])).observe_before_move(curr_state_);
            }
            move next_move = (*(players_[curr_player])).play(curr_state_);
            write_move_csv(moveFile, next_move);

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
                //return curr_score;
            } else if (curr_score == 25) {
                if (log_game) std::cout << "GAME WON!" << std::endl;
                //return curr_score;
            }
        }
        if (log_game) std::cout << "GAME FINISHED. SCORE:" << curr_score << std::endl;
        //return curr_score;
    } else {
        if (log_game) std::cout << "GAME WON!" << std::endl;
        //return curr_score;
    }
    //combine_csv(outFile, "decks.csv", "piles.csv", "hands.csv", "moves.csv", turn);
    // if (mkdir("output") == -1)
    //     std::cout << "output already exists" << std::endl;

    tree_csv(outFile, outFile2, "decks.csv", "piles.csv", "hands.csv", "moves.csv", turn);
    handFile.close();
    moveFile.close();
    pileFile.close();
    deckFile.close();
    outFile.close();
    return curr_score;
}

int game::run_test(bool log_game, std::vector<move> test_moves) {
    int curr_score = 0;
    if (log_game) std::cout << "STARTING TEST GAME\n" << std::endl;
    int curr_player = 0;
    int turn = 1;
    std::ofstream handFile("hands.csv");
    std::ofstream moveFile("moves.csv");
    std::ofstream pileFile("piles.csv");
    std::ofstream deckFile("decks.csv");
    std::ofstream outFile("p1.csv");
    std::ofstream outFile2("p2.csv");

    while ((curr_score < 25) && (curr_state_.get_num_lives() > 0) && !(curr_state_.get_deck().empty())) {
        if (log_game) std::cout << "TURN " << turn << " PLAYER " << curr_player << std::endl;
        

        for (int i = 0; i < players_.size(); i++) {
            State s = curr_state_;
            (*(players_[i])).observe_before_move(curr_state_);
        }
        write_hand_csv(handFile, curr_state_, curr_player, players_.size());
        write_pile_csv(pileFile, curr_state_);
        write_deck_csv(deckFile, curr_state_);

        move chosen = test_moves.back();
        test_moves.pop_back();
        //std::vector<move> legal_moves = (players_[curr_player])->get_legal_moves(curr_state_, curr_player);
        move next_move = (*(players_[curr_player])).play_prechosen(curr_state_, chosen); //here

        //write move csv
        write_move_csv(moveFile, next_move);

        //next_move = chosen; 


        // int c = count(legal_moves.begin(), legal_moves.end(), chosen);
        // std::cout << c << "helo" << std::endl;
        // if (c== 1){
        //     throw std::invalid_argument( "received invalid move" );
        // }


        if (log_game) next_move.str();

        for (int i = 0; i < players_.size(); i++) {
            State s = curr_state_;
            (*(players_[i])).observe(curr_state_, next_move); //broken here
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
            write_hand_csv(handFile, curr_state_, curr_player, players_.size());
            write_pile_csv(pileFile, curr_state_);
            write_deck_csv(deckFile, curr_state_);
            if (log_game) std::cout << "TURN " << turn << " PLAYER " << curr_player << std::endl;
            for (int i = 0; i < players_.size(); i++) {
                State s = curr_state_;

                (*(players_[i])).observe_before_move(curr_state_);
            }
            
            move chosen = test_moves.back();
            test_moves.pop_back();
            // std::vector<move> legal_moves = (players_[curr_player])->get_legal_moves(curr_state_, curr_id);
            move next_move = (*(players_[curr_player])).play_prechosen(curr_state_, chosen); //here
            write_move_csv(moveFile, next_move);
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
                //return curr_score;
            } else if (curr_score == 25) {
                if (log_game) std::cout << "GAME WON!" << std::endl;
                //return curr_score;
            }
        }
        if (log_game) std::cout << "GAME FINISHED. SCORE:" << curr_score << std::endl;
        //return curr_score;
    } else {
        if (log_game) std::cout << "GAME WON!" << std::endl;
        //return curr_score;
    }

    //close csv
    //combine_csv(outFile, "decks.csv", "piles.csv", "hands.csv", "moves.csv", turn);
    tree_csv(outFile, outFile2, "decks.csv", "piles.csv", "hands.csv", "moves.csv", turn);
    handFile.close();
    moveFile.close();
    pileFile.close();
    deckFile.close();
    outFile.close();
    return curr_score;
}