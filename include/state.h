#include "card.h"
#include "move.h"
#include <vector>
#include <set>

#pragma once
#include <map>
#include <string>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h> 
#include <pybind11/stl_bind.h>

namespace py = pybind11;

struct Observation {
    std::vector<std::vector<std::pair<std::string, int>>> hands;

    std::map<std::string, int> fireworks;

    int hint_tokens;    
    int lives_remaining;  
    int deck_size;       

    std::vector<std::pair<std::string, int>> discards;
    std::vector<py::tuple> legal_actions;

    int current_player_id;
};

#pragma once
class State {

    int num_players_;
    int cards_per_hand_;
    std::vector<std::vector<Card>> hands_;
    int hint_tokens_;
    int lives_;
    std::vector<Card> deck_;
    std::vector<Card> init_deck_; // for reset()
    std::vector<Card> discards_;
    std::vector<int> piles_; // rank of top most card in each pile (0 if pile is empty). Ignore piles[0]

public:
    State(int num_players);
    State(int num_players, std::vector<Card> deck); // provide an already shuffled deck

    // getters
    std::vector<move> get_legal_moves(int id) const;
    std::vector<std::vector<Card>> get_hands() const;
    int get_num_hints() const;
    int get_num_lives() const;
    std::vector<Card> get_discards() const;
    std::vector<int> get_piles() const;
    std::vector<Card> get_deck() const;

    void transition(move m, bool log);
    void reset(); // for de-bugging/testing

    Observation get_observation(int player_id) const;
};
