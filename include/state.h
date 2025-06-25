#include "card.h"
#include "move.h"
#include <vector>
#include <set>
#include <map>
#include <string>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>

namespace py = pybind11;

struct Observation {
    std::vector<std::vector<std::pair<std::string, int>>> hands;

    // New fields for card knowledge
    std::vector<std::vector<std::vector<bool>>> possible_colors;  // [player][card_index][color] -> whether this color is possible
    std::vector<std::vector<std::vector<bool>>> possible_ranks;   // [player][card_index][rank] -> whether this rank is possible

    std::map<std::string, int> fireworks;

    int hint_tokens;
    int lives_remaining;
    int deck_size;

    std::vector<std::pair<std::string, int>> discards;
    std::vector<std::tuple<int, int, int, int, std::vector<int>, int, int>> legal_actions;

    int current_player_id;

    // New field for vector encoding
    std::vector<int> vector_encoding;  // Bit vector encoding of the observation for the current player

    // Default constructor
    Observation() : hint_tokens(0), lives_remaining(0), deck_size(0), current_player_id(0) {}
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

    // New fields for tracking card knowledge
    std::vector<std::vector<std::vector<bool>>> possible_colors_;  // [player][card_index][color]
    std::vector<std::vector<std::vector<bool>>> possible_ranks_;   // [player][card_index][rank]

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
    const std::vector<std::vector<std::vector<bool>>>& get_possible_colors() const { return possible_colors_; }
    const std::vector<std::vector<std::vector<bool>>>& get_possible_ranks() const { return possible_ranks_; }

    void transition(move m, bool log);
    void reset(); // for de-bugging/testing
    void update_card_knowledge(const move& m); // New function to update card knowledge after hints

    Observation get_observation(int player_id) const;

    friend void EncodeHands(const State& state, int player_id, std::vector<int>& encoding, int& offset);
    friend void EncodeBoard(const State& state, std::vector<int>& encoding, int& offset);
    friend void EncodeDiscards(const State& state, std::vector<int>& encoding, int& offset);
};
