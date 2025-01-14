#include <iostream>
#include <stdio.h>
#include <algorithm>
#include <random>
#include <map>

#include "../include/state.h"

State::State(int num_players) {

    deck_ = { Card(red, one), Card(red, one), Card(red, one), Card(red, two), Card(red, two), Card(red, three), Card(red, three), Card(red, four), Card(red, four), Card(red, five), 
              Card(blue, one), Card(blue, one), Card(blue, one), Card(blue, two), Card(blue, two), Card(blue, three), Card(blue, three), Card(blue, four), Card(blue, four), Card(blue, five),
              Card(yellow, one), Card(yellow, one), Card(yellow, one), Card(yellow, two), Card(yellow, two), Card(yellow, three), Card(yellow, three), Card(yellow, four), Card(yellow, four), Card(yellow, five),
              Card(green, one), Card(green, one), Card(green, one), Card(green, two), Card(green, two), Card(green, three), Card(green, three), Card(green, four), Card(green, four), Card(green, five),
              Card(white, one), Card(white, one), Card(white, one), Card(white, two), Card(white, two), Card(white, three), Card(white, three), Card(white, four), Card(white, four), Card(white, five)};

    // Shuffling the deck
    std::random_device rd;
    std::default_random_engine gen(rd());
    std::shuffle(deck_.begin(), deck_.end(), gen);

    num_players_ = num_players;
    hint_tokens_ = 8;
    lives_ = 3;

    init_deck_ = {}; // to reset the state
    for (int i = 0; i < deck_.size(); i++) init_deck_.push_back(deck_[i]);

    cards_per_hand_ = (num_players <= 3) ? 5 : 4;
    // Distributing cards to players
    hands_ = {};
    for (int p = 0; p < num_players_; p++) {
        std::vector<Card> hand = {};
        for (int n = 0; n < cards_per_hand_; n++) {
            hand.push_back(deck_.back());
            deck_.pop_back();
        }
        hands_.push_back(hand);
    }
    discards_ = {};
    piles_ = {0, 0, 0, 0, 0, 0}; // all piles are empty
}

std::vector<move> State::get_legal_moves(int id){
    std::vector<move> moves;
    moves = {};

    if (get_num_hints() > 0) { // Can perform hints
        for (int i = 0; i < hands_.size(); i++) {
            if (i == id) continue; // Can't self-hint

            std::set<Color> colors; // Colors I can hint for this partner
            std::set<Rank> ranks; // ^
            std::map<Color, std::vector<int>> col_to_indices{}; // Color to the indices of cards of that color in partner i's hand
            std::map<Rank, std::vector<int>> rank_to_indices{}; // ^
            
            // Popylating the sets and maps
            for (int j = 0; j < hands_[i].size(); j++) {
                Card c = hands_[i][j];
                colors.insert(c.color());
                if (auto search = col_to_indices.find(c.color()); search == col_to_indices.end()) {
                    col_to_indices.insert({c.color(), {j}});
                } else {
                    col_to_indices.at(c.color()).push_back(j);
                }
                ranks.insert(c.rank());
                if (auto search = rank_to_indices.find(c.rank()); search == rank_to_indices.end()) {
                    rank_to_indices.insert({c.rank(), {j}});
                } else {
                    rank_to_indices.at(c.rank()).push_back(j);
                }
            }

            for (Color col : colors) moves.push_back(move(COL_HINT, i, id, col_to_indices.at(col), col));
            for (Rank rank : ranks) moves.push_back(move(RANK_HINT, i, id, rank_to_indices.at(rank), rank));
        }
    }
    
    if (get_num_hints() < 8) { // Can perform discards
        for (int i = 0; i < hands_[id].size(); i++) moves.push_back(move(DISCARD, id, i));
    }
    
    for (int i = 0; i < hands_[id].size(); i++) moves.push_back(move(PLAY, id, i));
    
    return moves;
}

State::State(int num_players, std::vector<Card> deck) {
    num_players_ = num_players;
    hint_tokens_ = 8;
    lives_ = 3;
    deck_ = deck; // pre-set deck
    init_deck_ = {};
    for (int i = 0; i < deck_.size(); i++) init_deck_.push_back(deck_[i]);
    cards_per_hand_ = (num_players <= 3) ? 5 : 4;
    // Distributing cards to players
    hands_ = {};
    for (int p = 0; p < num_players_; p++) {
        std::vector<Card> hand = {};
        for (int n = 0; n < cards_per_hand_; n++) {
            hand.push_back(deck_.back());
            deck_.pop_back();
        }
        hands_.push_back(hand);
    }
    discards_ = {};
    piles_ = {0, 0, 0, 0, 0, 0}; // all piles are empty
}

void State::transition(move m, bool log) {
    if (m.get_type() == DISCARD) {
        Card discard = hands_[m.get_from()][m.get_card_index()];
        discards_.push_back(discard);

        auto it = std::find(hands_[m.get_from()].begin(), hands_[m.get_from()].end(), 
                            discard);
        hands_[m.get_from()].erase(it); // remove the discarded card form the player's hand
        
        if (!(deck_.empty())) { // draw if deck isn't empty
            if (log) std::cout << std::endl << "        Draw: " << deck_.back().str() << std::endl;
            hands_[m.get_from()].push_back(deck_.back());
            deck_.pop_back();
        }
        hint_tokens_++; // if there were 8 hint tokens, a discard shoudn't have even been made
    } else if (m.get_type() == PLAY) {
        Card playing_card = hands_[m.get_from()][m.get_card_index()];
        auto it = std::find(hands_[m.get_from()].begin(), hands_[m.get_from()].end(), playing_card);
        hands_[m.get_from()].erase(it); // remove the played card form the player's hand

        int top_rank = piles_[playing_card.color()];
        if (playing_card.rank() == top_rank + 1) { // SUCCESSFUL PLAY
            piles_[playing_card.color()] += 1;
            if (log) std::cout << ", SUCCESS" << std::endl;

            if ((playing_card.rank() == five) && (hint_tokens_ < 8)) {
                hint_tokens_++; // get a hint back if a pile is completed
            }
        } else { // FAILED PLAY
            discards_.push_back(playing_card);
            lives_--;
            if (log) std::cout << ", FAILED" << std::endl;
        }

        if (!(deck_.empty())) { // draw if deck isn't empty
            if (log) std::cout << "        DRAW: " << deck_.back().str() << std::endl;
            hands_[m.get_from()].push_back(deck_.back());
            deck_.pop_back();
        }
    } else { // A hint was given
        hint_tokens_--;
        if (log) std::cout << std::endl;
    }
    if (log) {
        std::cout << "    States after Moves: " << std::endl;
        std::cout << "        Number of Hints: " << std::to_string(hint_tokens_) << std::endl;
        std::cout << "        Number of Lives: " << std::to_string(lives_) << std::endl;
        std::cout << "        Deck Size: " << deck_.size() << std::endl;
        static const std::array<std::string, 5> color_names = {
                "red", "white", "yellow", "green", "blue"
        };

        std::cout << "        Piles: ";
        for (size_t i = 1; i < piles_.size(); ++i) {
            std::cout << color_names[i - 1] << " " << std::to_string(piles_[i]);
            if (i < piles_.size() - 1) {
                std::cout << ", ";
            }
        }
        std::cout << std::endl;
    }
}

int State::get_num_hints() const {
    return hint_tokens_;
}
int State::get_num_lives() const {
    return lives_;
}
std::vector<Card> State::get_discards() const {
    return discards_;
}
std::vector<int> State::get_piles() const {
    return piles_;
}
std::vector<Card> State::get_deck() const {
    return deck_;
}

std::vector<std::vector<Card>> State::get_hands() const {
    return hands_;
}

void State::reset() {
    deck_ = init_deck_;
    lives_ = 3;
    hint_tokens_ = 8;
    hands_ = {};
    for (int p = 0; p < num_players_; p++) { // disttibute cards again
        std::vector<Card> hand = {};
        for (int n = 0; n < cards_per_hand_; n++) {
            hand.push_back(deck_.back());
            deck_.pop_back();
        }
        hands_.push_back(hand);
    }
    discards_ = {};
    piles_ = {0, 0, 0, 0, 0, 0};
}