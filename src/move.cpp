#include <iostream>
#include <stdio.h>


#include "../include/move.h"

move::move(move_type type, int to, int from, int card_index, std::vector<int> card_indices, Color color, Rank rank) : 
    type_(type), to_(to), from_(from), card_index_(card_index), card_indices_(card_indices), color_(color), rank_(rank) {
}

move_type move::get_type() {
    return type_;
}

int move::get_to() {
    return to_;
}

int move::get_from() {
    return from_;
}

int move::get_card_index() {
    return card_index_;
}

Color move::get_color() {
    return color_;
}

Rank move::get_rank() {
    return rank_;
}

std::vector<int> move::get_card_indices() {
    return card_indices_;
}

std::string col_str(Color col) {
    if (col == red) return "red";
    if (col == blue) return "blue";
    if (col == yellow) return "yellow";
    if (col == green) return "green";
    if (col == white) return "white";
    if (col == invalid_color) return "invalid";
    return "";
}

int rank_num(Rank r) {
    int num = 0;
    switch(r) {
        case one: num = 1; break;
        case two: num = 2; break;
        case three: num = 3; break;
        case four: num = 4; break;
        case five: num = 5; break;
    }
    return num;
}

void move::str() {
    if (type_ == COL_HINT) {
        std::cout << col_str(color_) << " COLOR HINT to player " << to_ << std::endl;
    } else if (type_ == RANK_HINT) {
        std::cout << rank_num(rank_) << " RANK HINT to player " << to_ << std::endl;
    } else if (type_ == PLAY) {
        std::cout << "PLAY CARD NO. " << card_index_ + 1 << std::endl;
    } else {
        std::cout << "DISCARD CARD NO. " << card_index_ + 1 << std::endl;
    }
}
