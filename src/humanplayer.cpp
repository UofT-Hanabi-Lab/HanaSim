#include <random>
#include <set>
#include <algorithm>
#include <iterator>
#include <iostream>
#include "../include/humanplayer.h"
#include "../include/move.h"



humanplayer::humanplayer(int num_players, int id) : num_cards_((num_players <= 3) ? 5 : 4), id_(id) {
}

void humanplayer::observe(State s, move m) {};

void humanplayer::observe_before_move(State s) {};

int humanplayer::get_id() {
    return id_;
}

int humanplayer::get_n_cards() {
    return num_cards_;
}

std::string col_convert(Color col) {
    if (col == red) return "red";
    if (col == blue) return "blue";
    if (col == yellow) return "yellow";
    if (col == green) return "green";
    if (col == white) return "white";
    if (col == invalid_color) return "invalid";
    return "";
}

int rank_convert(Rank r) {
    int num = 0;
    switch(r) {
        case one: num = 1; break;
        case two: num = 2; break;
        case three: num = 3; break;
        case four: num = 4; break;
        case five: num = 5; break;
        case invalid_rank: break;
    }
    return num;
}

void humanplayer::print_move(move m, int index) { // a bit different than move::str
    move_type type = m.get_type();
    int card_index = m.get_card_index();
    Color color = m.get_color();
    int to = m.get_to();
    Rank rank = m.get_rank();
    if (type == COL_HINT) {
        std::cout << index << ": " << col_convert(color) << " COLOR HINT to player " << to << std::endl;
    } else if (type == RANK_HINT) {
        std::cout << index << ": " << rank_convert(rank) << " RANK HINT to player " << to << std::endl;
    } else if (type == PLAY) {
        std::cout << index << ": " << "PLAY CARD NO. " << card_index + 1 << std::endl;
    } else {
        std::cout << index << ": " << "DISCARD CARD NO. " << card_index + 1 << std::endl;
    }
}

void humanplayer::print_hands(State s) {
    std::vector<std::vector<Card>> hands = s.get_hands();
    int curr_player = 0;
    for (auto & cards : hands){
        std::cout << "Hand of Player: " << curr_player <<std::endl;
        curr_player++;
        for (auto & c : cards) {
            std::cout << col_convert(c.color()) << " "
            << rank_convert(c.rank()) <<std::endl;
        }
    }
}

move humanplayer::play(State s) {
    std::vector<move> legal_moves = s.get_legal_moves(id_);
    int move_index = 0;
    print_hands(s);
    for (auto & m : legal_moves) {
        print_move(m, move_index);
        move_index++;
    }
    std::cout << "select which move to make ";
    int num;
    std::cin >> num;
    std::cout << "selected move " << num <<std::endl;
    return legal_moves[num];
}


move humanplayer::play_prechosen(State s, move chosen_m) {
    std::vector<move> legal_moves = s.get_legal_moves(id_);
    int move_index = 0;
    print_hands(s);
    bool valid = false;
    for (auto & element : legal_moves) {
        if(valid == false){
            valid = element.compare(chosen_m);
        }
        print_move(element, move_index);
        move_index++;
    }
    if (valid == false){
        throw std::invalid_argument("received invalid move");
    }
    std::cout << "valid move chosen";

    return chosen_m;
}