/*


A class for a Hanabi move.


*/
#include <vector>
#include "card.h"

#pragma once

enum move_type { COL_HINT=1, RANK_HINT, PLAY, DISCARD, INVALID_MOVE };
class State;

class move 
{
    move_type type_;
    int to_; // id of player that you're hinting to. -1 if move isn't a hint
    int from_; // id of active player
    int card_index_; // card index you want to play/discard, -1 if move is a hint
    std::vector<int> card_indices_; // card indices that were hinted, empty if not a hint
    Color color_; // invalid_color if not a color hint
    Rank rank_; // invalid_rank if not a rank hint

public:
    // Different constructors for the different move_types
    move(move_type type);
    move(move_type type, int from, int card_index);  // Discard, Play
    move(move_type type, int to, int from, std::vector<int> card_indices, Color color);  // Hint for color
    move(move_type type, int to, int from, std::vector<int> card_indices, Rank rank);  // Hint for rank

    // GETTERS
    move_type get_type() const;
    int get_to() const;
    int get_from() const;
    int get_card_index() const;
    Color get_color() const;
    Rank get_rank() const;
    std::vector<int> get_card_indices() const;
    
    bool compare(move m);
    std::string str();
    std::string str(State s);
};