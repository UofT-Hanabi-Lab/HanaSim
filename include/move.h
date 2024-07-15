/*


A class for a Hanabi move.


*/
#include <vector>
#include "card.h"

#pragma once

enum move_type { COL_HINT=1, RANK_HINT, PLAY, DISCARD, INVALID_MOVE };
class move 
{
    move_type type_;
    int to_; // id of player hinting to. -1 if type != COL_HINT and type != RANK_HINT
    int from_; // id of active player
    int card_index_; // card index you want to play/discard, -1 if move is a hint
    std::vector<int> card_indices_; // card indices that were hinted, empty if not a hint
    Color color_; // invalid_color if not a color hint
    Rank rank_; // invalid_rank if not a rank hint

public:
    move(move_type type);
    move(move_type type, int from, int card_index);
    move(move_type type, int to, int from, std::vector<int> card_indices, Color color);
    move(move_type type, int to, int from, std::vector<int> card_indices, Rank rank);
    move_type get_type();
    int get_to();
    int get_from();
    int get_card_index();
    Color get_color();
    Rank get_rank();
    std::vector<int> get_card_indices();
    void str();
};