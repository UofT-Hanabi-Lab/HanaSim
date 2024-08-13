#include <random>
#include <set>
#include <algorithm>
#include <iterator>
#include <iostream>
#include "include/humanplayer.h"
#include "../include/move.h"



humanplayer::humanplayer(int id, int num_players) : num_cards_((num_players <= 3) ? 5 : 4), id_(id) {
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

void humanplayer::print_move(move m, int index){
//    move::move(move_type type, int to, int from, int card_index, Color color, Rank rank) :
//    type_(type), to_(to), from_(from), card_index_(card_index), color_(color), rank_(rank) {
//    }
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
        for (auto & element : cards) {
            std::cout << col_convert(element.color()) << " "
            << rank_convert(element.rank()) <<std::endl;
        }
    }
}


std::vector<move> humanplayer::get_legal_moves(State s, int id) {
    std::vector<move> moves;
    moves = {};
    std::vector<std::vector<Card>> hands = s.get_hands();
    if (s.get_num_hints() > 0) {
        for (int i = 0; i < hands.size(); i++) {
            std::set<Color> colors;
            std::set<Rank> ranks;
            std::map<Color, std::vector<int>> col_to_indices{};
            std::map<Rank, std::vector<int>> rank_to_indices{};
            if (i == id) {
                continue;
            }
            std::cout << hands[i].size() << std::endl;
            for (int j = 0; j < hands[i].size(); j++) {
                Card c = hands[i][j];
                colors.insert(c.color());
                if (auto search = col_to_indices.find(c.color()); search == col_to_indices.end()) {
                    col_to_indices.insert({c.color(), {j}});
                } else {
                    col_to_indices.at(c.color()).push_back(j);
                }
                ranks.insert(c.rank());
                if (auto search = rank_to_indices.find(c.rank()); search == rank_to_indices.end()) {
                    rank_to_indices.insert({c.rank(), {j}});
                } else {
                    rank_to_indices.at(c.rank()).push_back(j);
                }
            }
            for (Color col : colors) {
                moves.push_back(move(COL_HINT, i, id, col_to_indices.at(col), col));
            }
            for (Rank rank : ranks) moves.push_back(move(RANK_HINT, i, id, rank_to_indices.at(rank), rank));
        }
    }
    
    if (s.get_num_hints() < 8) {
        for (int i = 0; i < hands[id].size(); i++) {
            moves.push_back(move(DISCARD, id, i));
        }
    }
    
    for (int i = 0; i < hands[id].size(); i++) {
        moves.push_back(move(PLAY, id, i));
    }
    
    return moves;
}

move humanplayer::play(State s) {
    std::vector<move> legal_moves = get_legal_moves(s, id_);
    std::vector<move> m;
    int move_index = 0;
    print_hands(s);
    for (auto & element : legal_moves) {
        print_move(element, move_index);
        move_index++;
    }
    std::cout  << "select which move to make ";
    int num;
    std::cin >> num;
    std::cout << "selected move " << num <<std::endl;
    return legal_moves[num];
}