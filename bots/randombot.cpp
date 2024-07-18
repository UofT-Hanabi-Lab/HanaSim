#include <random>
#include <set>
#include <algorithm>
#include <iterator>
#include <iostream>


#include "../include/randombot.h"




randombot::randombot(int n_cards, int id) : num_cards_(n_cards), id_(id) {    
}


void randombot::observe(State s, move m) {};


int randombot::get_id() {
    return id_;
}


int randombot::get_n_cards() {
    return num_cards_;
}


std::vector<move> randombot::get_legal_moves(State s, int id) {
    std::vector<move> moves;
    moves = {};
    std::vector<std::vector<Card>> hands = s.get_hands();
    if (s.get_num_hints() > 0) {
        for (int i = 0; i < hands.size(); i++) {
            std::set<Color> colors;
            std::set<Rank> ranks;
            if (i == id) {
                continue;
            }
            for (int j = 0; j < hands[i].size(); j++) {
                Card c = hands[i][j];
                colors.insert(c.color());
                ranks.insert(c.rank());
            }
            for (Color col : colors) {
                std::vector<int> indices;
                for (int j = 0; j < hands[i].size(); j++) {
                    if (hands[i][j].color() == col) indices.push_back(j);
                }
                moves.push_back(move(COL_HINT, i, id, indices, col));
            };
            for (Rank rank : ranks) {
                std::vector<int> indices;
                for (int j = 0; j < hands[i].size(); j++) {
                    if (hands[i][j].rank() == rank) indices.push_back(j);
                }
                moves.push_back(move(RANK_HINT, i, id, indices, rank));
            }
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


move randombot::play(State s) {
    std::vector<move> legal_moves = get_legal_moves(s, id_);
    std::vector<move> m;
    std::sample(legal_moves.begin(), legal_moves.end(), std::back_inserter(m), 1, std::mt19937{std::random_device{}()});
    return m[0];
}
