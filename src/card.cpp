#include <iostream>
#include <stdio.h>


#include "../include/card.h"



Card::Card(Color color, Rank rank) : color_(color), rank_(rank) {
}

Color Card::color() {
    return color_;
}

Rank Card::rank() {
    return rank_;
}

void Card::str() {

    // find out how to properly change colour
    std::string col_str = "";
    int rank_int = 0;
    // std::cout << "-------" << std::endl;
    // std::cout << "|" << rank_ << "    |" << std::endl;
    // std::cout << "|     |" << std::endl;
    // std::cout << "|     |" << std::endl;
    // std::cout << "|    " << rank_ << "|" << std::endl;
    // std::cout << "-------" << std::endl;
    if (color_ == red) col_str = "red";
    if (color_ == green) col_str = "green";
    if (color_ == blue) col_str = "blue";
    if (color_ == white) col_str = "white";
    if (color_ == yellow) col_str = "yellow";

    if (rank_ == one) rank_int = 1;
    if (rank_ == two) rank_int = 2;
    if (rank_ == three) rank_int = 3;
    if (rank_ == four) rank_int = 4;
    if (rank_ == five) rank_int = 5;
    std::cout << col_str << rank_int << std::endl;
}

bool Card::operator == (const Card c) {
    return ((color_ == c.color_) && (rank_ == c.rank_));
}