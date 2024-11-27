#include <iostream>
#include <stdio.h>
#include <numeric>
#include <string>
#include <fstream>
#include <string>
#include <direct.h> // for _mkdir
#include "../include/global_int.h"

#include "../include/game.h"
#include "../include/humanplayer.h"
#include "../bots/include/holmesbot.h"
#include "../bots/include/smartbot.h"


State game::get_curr_state() {
    return curr_state_;
}

game::game(State init_state, std::vector<player*> players) : curr_state_(init_state), players_(players){
}

// int game::create_hand_knowledge(std::vector<player*> players, int curr_player){
//     //HANDKNOWLEDGE
//     State s = curr_state_;
//     int num_players = players_.size();
//     int num_cards_ = (num_players <= 3) ? 5 : 4;;
//     std::vector<int> hk_playable = {};
//     hk_playable.resize(num_players * num_cards_);
//     std::vector<int> hk_valuable = {};
//     hk_valuable.resize(num_players * num_cards_);
//     std::vector<int> hk_worthless = {};
//     hk_worthless.resize(num_players * num_cards_);
//     std::vector<int> hk_cols = {};
//     hk_cols.resize(num_players * num_cards_);
//     std::vector<int> hk_ranks = {};
//     hk_ranks.resize(num_players * num_cards_);


//     if (dynamic_cast<smartbot*>(players[curr_player]) != nullptr) {
//         smartbot *smart_partner = (smartbot*)(players[curr_player]);
//         std::vector<std::vector<SmartBotInternal::smart_cardknowledge>> hk = smart_partner->get_hk();
//         for (int i = 0; i < hk.size(); i++) {
//             for (int j = 0; j < hk[i].size(); j++) {
//                 hk_playable.push_back((hk[i][j].playable(s) == SmartBotInternal::NO) ? 0 : ((hk[i][j].get_playable() == SmartBotInternal::YES) ? 1 : 2));
//                 hk_valuable.push_back((hk[i][j].valuable(s) == SmartBotInternal::NO) ? 0 : ((hk[i][j].get_valuable() == SmartBotInternal::YES) ? 1 : 2));
//                 hk_worthless.push_back((hk[i][j].worthless(s) == SmartBotInternal::NO) ? 0 : ((hk[i][j].get_worthless() == SmartBotInternal::YES) ? 1 : 2));
//                 hk_cols.push_back((hk[i][j].color() < 0) ? 6 : hk[i][j].color());
//                 hk_cols.push_back((hk[i][j].rank() < 0) ? 6 : hk[i][j].rank());
//             }
//         }
//     } 
//     else if (dynamic_cast<holmesbot*>(players[curr_player]) != nullptr) {
//         holmesbot *holmes_partner = (holmesbot*)(players[curr_player]);
//         std::vector<std::vector<HolmesBotInternal::holmes_cardknowledge>> hk = holmes_partner->get_hk();
//         for (int i = 0; i < hk.size(); i++) {
//             for (int j = 0; j < hk[i].size(); j++) {
//                 hk_playable.push_back((int)hk[i][j].is_playable);
//                 hk_valuable.push_back((int)hk[i][j].is_valuable);
//                 hk_worthless.push_back((int)hk[i][j].is_worthless);
//                 hk_cols.push_back((int)(hk[i][j].color()));
//                 hk_cols.push_back((int)(hk[i][j].rank()));
//             }
//         }
//     }
// }

int game::write_move_csv(std::ofstream &moveFile, move next_move){
    //moveFile << "[";
    if(next_move.get_type() == 1){
        moveFile << next_move.get_type() << next_move.get_to() << next_move.get_from();
        // moveFile << "[";
        // std::vector<int> indices = next_move.get_card_indices();
        // for(int x = 0; x < indices.size(); x++){
        //     moveFile << indices[x];
        //     if(x != indices.size() - 1) moveFile << "; ";
        // }
        moveFile << next_move.get_color();  
    }
    if(next_move.get_type() == 2){
        moveFile << next_move.get_type() << next_move.get_to() << next_move.get_from();
        // moveFile << "[";
        // std::vector<int> indices = next_move.get_card_indices();
        // for(int x = 0; x < indices.size(); x++){
        //     moveFile << indices[x];
        //     if(x != indices.size() - 1) moveFile << "; ";
        // }
        moveFile << next_move.get_rank();    
    }
    if(next_move.get_type() == 3 || next_move.get_type() == 4){
        moveFile << next_move.get_type() << next_move.get_from() << next_move.get_card_index();  
    }
    moveFile << std::endl;
    return 0; 
}

int game::write_hand_csv(std::ofstream &handFile, State s, int curr_p, int num_p){
    //handFile << "[";
    for (int x = 0; x < num_p; x++) {
        // for writing to csv
        std::vector<std::vector<Card>> hands = s.get_hands();
        //std::cout << "player " << x << std::endl;
        for (int y=0; y < hands[x].size(); y++){
            if(x == curr_p){
                    hands[x][y] = Card(invalid_color, invalid_rank);
            }
            //std::cout << hands[x][y].color() << hands[x][y].rank() << std::endl;
            handFile << hands[x][y].color() << ", "<< hands[x][y].rank();
            if(y != hands[x].size() - 1) handFile << ", ";
        }
        if(x != hands.size() - 1) handFile << ", ";
    }
    handFile << std::endl;
    return 0;
}

int game::write_pile_csv(std::ofstream &pileFile, State s){
    std::vector<int> piles = curr_state_.get_piles();
    //pileFile << "[";
    // start at index 1, apparently index 0 is for invalid card pile?
    for (int x = 1; x < piles.size(); x++){
        pileFile << piles[x];
        if(x != piles.size() - 1) pileFile << ", ";
    }
    pileFile << std::endl;
    return 0;
}

int game::write_deck_csv(std::ofstream &deckFile, State s){
    std::vector<Card> deck = s.get_deck();
    int hints = s.get_num_hints();
    std::vector<Card> d = s.get_discards();
    //deckFile << "[";
    std::vector<int> discards;
    for (int x = 0; x < 25; x++){
        discards.push_back(0);
    }
    for (int x = 0; x < d.size(); x++){
        int color = d[x].color();
        int rank = d[x].rank();
        int index = ((color-1) * 5) + rank; 
        //std::cout << color << " " << rank << " " << index << std::endl;
        //std::cout << "discards:" << d[x].color() << d[x].rank() << std::endl;
        discards[index-1] += 1;
    }
    for (int x = 0; x < discards.size(); x++){
        deckFile << discards[x] << ", ";
        //if(x != discards.size() - 1) deckFile << ", ";
    }
    deckFile << deck.size() << ", " << hints << std::endl;
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

int game::tree_csv(std::ofstream &outFile, std::ofstream &outFile2, std::string dfile, std::string pfile, std::string hfile, 
    std::string mfile, std::string hkfile, int turn){
    //assume only 2 player
    std::ifstream handFile(hfile);
    std::ifstream moveFile(mfile);
    std::ifstream pileFile(pfile);
    std::ifstream deckFile(dfile);
    std::ifstream hkFile(hkfile);

    std::string deck;
    std::string pile;
    std::string hand;
    std::string move;
    std::string hk;

    std::vector<std::string> attributes = 
    {"hand1_col1", "hand1_rank1", "hand1_col2", "hand1_rank2", "hand1_col3", "hand1_rank3", "hand1_col4", "hand1_rank4", "hand1_col5", "hand1_rank5",
     "hand2_col1", "hand2_rank1", "hand2_col2", "hand2_rank2", "hand2_col3", "hand2_rank3", "hand2_col4", "hand2_rank4", "hand2_col5", "hand2_rank5",
     "discard11", "discard12", "discard13", "discard14", "discard15", 
     "discard21", "discard22", "discard23", "discard24", "discard25", 
     "discard31", "discard32", "discard33", "discard34", "discard35", 
     "discard41", "discard42", "discard43", "discard44", "discard45", 
     "discard51", "discard52", "discard53", "discard54", "discard55", 
    "deck_count", "hints", "pile1", "pile2", "pile3", "pile4", "pile5",  "move",
    "hk_h1_p1", "hk_h1_v1", "hk_h1_w1", "hk_h1_c1", "hk_h1_r1",
    "hk_h1_p2", "hk_h1_v2", "hk_h1_w2", "hk_h1_c2", "hk_h1_r2",
    "hk_h1_p3", "hk_h1_v3", "hk_h1_w3", "hk_h1_c3", "hk_h1_r3",
    "hk_h1_p4", "hk_h1_v4", "hk_h1_w4", "hk_h1_c4", "hk_h1_r4",
    "hk_h1_p5", "hk_h1_v5", "hk_h1_w5", "hk_h1_c5", "hk_h1_r5",
    "hk_h2_p1", "hk_h2_v1", "hk_h2_w1", "hk_h2_c1", "hk_h2_r1",
    "hk_h2_p2", "hk_h2_v2", "hk_h2_w2", "hk_h2_c2", "hk_h2_r2",
    "hk_h2_p3", "hk_h2_v3", "hk_h2_w3", "hk_h2_c3", "hk_h2_r3",
    "hk_h2_p4", "hk_h2_v4", "hk_h2_w4", "hk_h2_c4", "hk_h2_r4",
    "hk_h2_p5", "hk_h2_v5", "hk_h2_w5", "hk_h2_c5", "hk_h2_r5"}; 


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
        std::getline(hkFile, hk);
        if (x % 2 == 1){
            outFile << hand << ", " << deck << ", " << pile << ", " << move << ", " << hk << std::endl; 
        }
        else{
            outFile2 << hand << ", " << deck << ", " << pile << ", " << move << ", " << hk << std::endl; 
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
    if (mkdir("output/p1") == -1)
        std::cout << "failed to create directory or directory already exists" << std::endl;
    if (mkdir("output/p2") == -1)
        std::cout << "failed to create directory or directory already exists" << std::endl;
    std::string fname1 = "output/p1/g" + std::to_string(game_num) + ".csv";
    std::string fname2 = "output/p2/g" + std::to_string(game_num) + ".csv";
    std::ofstream handFile("output/hands.csv");
    std::ofstream moveFile("output/moves.csv");
    std::ofstream pileFile("output/piles.csv");
    std::ofstream deckFile("output/decks.csv");
    std::ofstream hkFile("output/hk.csv");
    std::ofstream outFile(fname1);
    std::ofstream outFile2(fname2);

    
    while ((curr_score < 25) && (curr_state_.get_num_lives() > 0) && !(curr_state_.get_deck().empty())) {
        if (log_game) std::cout << "TURN " << turn << " PLAYER " << curr_player << std::endl;
        
        // write csv
        for (int i = 0; i < players_.size(); i++) {
            State s = curr_state_;
            (*(players_[i])).observe_before_move(curr_state_);
        }

    //create handknowledge #####################################################################
    State s = curr_state_;
    int num_players = players_.size();
    int num_cards_ = (num_players <= 3) ? 5 : 4;;
    std::vector<int> hk_playable = {};
    std::vector<int> hk_valuable = {};
    std::vector<int> hk_worthless = {};
    std::vector<int> hk_cols = {};
    std::vector<int> hk_ranks = {};
    if (dynamic_cast<smartbot*>(players_[curr_player]) != nullptr) {
        std::cout << "casted to smart" << std::endl;
        smartbot *smart_partner = (smartbot*)(players_[curr_player]);
        std::vector<std::vector<SmartBotInternal::cardknowledge>> hk = smart_partner->get_hk();
        for (int i = 0; i < hk.size(); i++) {
            for (int j = 0; j < hk[i].size(); j++) {
                hk_playable.push_back((hk[i][j].playable(s) == SmartBotInternal::NO) ? 0 : ((hk[i][j].get_playable() == SmartBotInternal::YES) ? 1 : 2));
                hk_valuable.push_back((hk[i][j].valuable(s) == SmartBotInternal::NO) ? 0 : ((hk[i][j].get_valuable() == SmartBotInternal::YES) ? 1 : 2));
                hk_worthless.push_back((hk[i][j].worthless(s) == SmartBotInternal::NO) ? 0 : ((hk[i][j].get_worthless() == SmartBotInternal::YES) ? 1 : 2));
                hk_cols.push_back((hk[i][j].color() < 0) ? 6 : hk[i][j].color());
                hk_ranks.push_back((hk[i][j].rank() < 0) ? 6 : hk[i][j].rank());
            }
        }
    } 
    else if (dynamic_cast<holmesbot*>(players_[curr_player]) != nullptr) {
        std::cout << "casted to holmes" << std::endl;
        holmesbot *holmes_partner = (holmesbot*)(players_[curr_player]);
        std::vector<std::vector<HolmesBotInternal::cardknowledge>> hk = holmes_partner->get_hk();
        for (int i = 0; i < hk.size(); i++) {
            for (int j = 0; j < hk[i].size(); j++) {
                hk_playable.push_back((int)hk[i][j].is_playable);
                hk_valuable.push_back((int)hk[i][j].is_valuable);
                hk_worthless.push_back((int)hk[i][j].is_worthless);
                hk_cols.push_back((int)(hk[i][j].color()));
                hk_ranks.push_back((int)(hk[i][j].rank()));
            }
        }
    }
    // write hk csv #####################################################################
    for (int i = 0; i < hk_cols.size(); i++){
        hkFile << hk_playable[i] << ", " << hk_valuable[i] << ", " << hk_worthless[i] << ", " << hk_cols[i] 
        << ", " << hk_ranks[i];
        if(i != hk_cols.size() - 1) hkFile << ", ";
    }
    hkFile << std::endl;
    // #########################################################################
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

    tree_csv(outFile, outFile2, "output/decks.csv", "output/piles.csv", "output/hands.csv", "output/moves.csv", "output/hk.csv", turn);
    handFile.close();
    moveFile.close();
    pileFile.close();
    deckFile.close();
    outFile.close();
    hkFile.close();
    return curr_score;
}

int game::run_test(bool log_game, std::vector<move> test_moves) {
    int curr_score = 0;
    if (log_game) std::cout << "STARTING TEST GAME\n" << std::endl;
    int curr_player = 0;
    int turn = 1;
    if (mkdir("output/p1") == -1)
        std::cout << "failed to create directory or directory already exists" << std::endl;
    if (mkdir("output/p2") == -1)
        std::cout << "failed to create directory or directory already exists" << std::endl;
    std::string fname1 = "output/p1/g" + std::to_string(game_num) + ".csv";
    std::string fname2 = "output/p2/g" + std::to_string(game_num) + ".csv";
    std::ofstream handFile("output/hands.csv");
    std::ofstream moveFile("output/moves.csv");
    std::ofstream pileFile("output/piles.csv");
    std::ofstream deckFile("output/decks.csv");
    std::ofstream outFile(fname1);
    std::ofstream outFile2(fname2);

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
    tree_csv(outFile, outFile2, "output/decks.csv", "output/piles.csv", "output/hands.csv", "output/moves.csv", "output/hk.csv", turn);
    handFile.close();
    moveFile.close();
    pileFile.close();
    deckFile.close();
    outFile.close();
    return curr_score;
}