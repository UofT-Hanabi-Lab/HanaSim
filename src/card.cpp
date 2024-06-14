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

    std::cout << "-------" << std::endl;
    std::cout << "|" << rank_ << "    |" << std::endl;
    std::cout << "|     |" << std::endl;
    std::cout << "|     |" << std::endl;
    std::cout << "|    " << rank_ << "|" << std::endl;
    std::cout << "-------" << std::endl;

    system("color 00");
}

bool Card::operator == (const Card c) {
    return ((color_ == c.color_) && (rank_ == c.rank_));
}