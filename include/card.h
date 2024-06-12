/*

A class for a card in Hanabi.

*/

#pragma once

// possible colors
enum Color { red = 1, green, blue, yellow, white };
// possible ranks
enum Rank { one = 1, two, three, four, five };

class Card
{
    Color color_;
    Rank rank_;

public:

    Card(Color, Rank);

    Color color();

    Rank rank();

    void str();
};
