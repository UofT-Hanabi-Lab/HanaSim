/*

A class for a card in Hanabi.

*/

#pragma once

// possible colors
enum Color { red, green, blue, yellow, white };
// possible ranks
enum Rank { one, two, three, four, five };

class card
{

    Color color;
    Rank rank;

    
    void str();

};
