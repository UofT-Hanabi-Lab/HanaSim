/*

A class for a card in Hanabi.

*/

#pragma once

// possible colors
enum Color { red = 1, white, yellow, green, blue, invalid_color };
// possible ranks
enum Rank { one = 1, two, three, four, five, invalid_rank };

class Card
{
    Color color_;
    Rank rank_;

public:
    Card(Color, Rank);

    // Getters
    Color color();
    Rank rank();

    std::string str();
    bool operator == (const Card c);
};
