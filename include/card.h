/*

A class for a card in Hanabi.

*/

#pragma once

#include <string>

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
    Color color() const;
    Rank rank() const;

    std::string str() const;
    bool operator == (const Card c) const;

    bool operator<(const Card& other) const {
        if (color_ != other.color_) {
            return color_ < other.color_;
        }
        return rank_ < other.rank_;
    }
};
