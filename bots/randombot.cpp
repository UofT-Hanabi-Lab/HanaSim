#include <random>
#include <set>
#include <algorithm>
#include <iterator>
#include <iostream>

#include "../include/randombot.h"


randombot::randombot(int n_cards, int id) : num_cards_(n_cards), id_(id) {    
}

void randombot::observe(move m) {};

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
            for (Card c : hands[i]) {
                colors.insert(c.color());
                ranks.insert(c.rank());
            }
            for (Color col : colors) {

                moves.push_back(move(COL_HINT, i, id, -1, col, invalid_rank));
            };
            for (Rank rank : ranks) moves.push_back(move(RANK_HINT, i, id, -1, invalid_color, rank));
        }
    }
    if (s.get_num_hints() < 8) {
        for (int i = 0; i < hands[id].size(); i++) {
            moves.push_back(move(DISCARD, -1, id, i, invalid_color, invalid_rank));
        }
    }
    for (int i = 0; i < hands[id].size(); i++) {
        moves.push_back(move(PLAY, -1, id, i, invalid_color, invalid_rank));
    }
    return moves;
}

move randombot::play(State s) {
    std::vector<move> legal_moves = get_legal_moves(s, id_);
    std::vector<move> m;
    std::sample(legal_moves.begin(), legal_moves.end(), std::back_inserter(m), 1, std::mt19937{std::random_device{}()});
    return m[0];
}