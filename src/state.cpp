#include <iostream>
#include <stdio.h>
#include <algorithm>
#include <random>

#include "../include/state.h"

State::State(int num_players) {

    deck_ = { Card(red, one), Card(red, one), Card(red, one), Card(red, two), Card(red, two), Card(red, three), Card(red, three), Card(red, four), Card(red, four), Card(red, five), 
              Card(blue, one), Card(blue, one), Card(blue, one), Card(blue, two), Card(blue, two), Card(blue, three), Card(blue, three), Card(blue, four), Card(blue, four), Card(blue, five),
              Card(yellow, one), Card(yellow, one), Card(yellow, one), Card(yellow, two), Card(yellow, two), Card(yellow, three), Card(yellow, three), Card(yellow, four), Card(yellow, four), Card(yellow, five),
              Card(green, one), Card(green, one), Card(green, one), Card(green, two), Card(green, two), Card(green, three), Card(green, three), Card(green, four), Card(green, four), Card(green, five),
              Card(white, one), Card(white, one), Card(white, one), Card(white, two), Card(white, two), Card(white, three), Card(white, three), Card(white, four), Card(white, four), Card(white, five)};
    // for (int col = red; col != white; col++) {
    //     for (int rank = one; rank <= five; rank++) {
    //         int num_rank;
    //         switch(rank) {
    //             case one:
    //                 num_rank = 3;
    //                 break;
    //             case five:
    //                 num_rank = 1;
    //                 break;
    //             default:
    //                 num_rank = 2;
    //         }
    //         for (int i = 0; i < num_rank; i++) {
    //             deck_.push_back(Card(static_cast<Color>(col), static_cast<Rank>(rank)));
                
    //         }
    //     }
    // }
    // Shuffling the deck
    std::random_device rd;
    std::default_random_engine gen(rd());
    std::shuffle(deck_.begin(), deck_.end(), gen);
    num_players_ = num_players;
    hint_tokens_ = 8;
    lives_ = 3;
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

State::State(int num_players, std::vector<Card> deck) {
    num_players_ = num_players;
    hint_tokens_ = 8;
    lives_ = 3;
    deck_ = deck;
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
        hands_[m.get_from()].erase(it);
        if (!(deck_.empty())) {
            if (log) deck_.back().str();
            hands_[m.get_from()].push_back(deck_.back());
            deck_.pop_back();
        }
        hint_tokens_++; // if there were 8 hint tokens, a discard shoudn't have even been made
    } else if (m.get_type() == PLAY) {
        Card playing_card = hands_[m.get_from()][m.get_card_index()];
        int top_rank = piles_[playing_card.color()];
        if (playing_card.rank() == top_rank + 1) { // SUCCESSFUL PLAY
            piles_[playing_card.color()] += 1;
            auto it = std::find(hands_[m.get_from()].begin(), hands_[m.get_from()].end(), 
                                playing_card);
            hands_[m.get_from()].erase(it);
            if (!(deck_.empty())) {
                if (log) deck_.back().str();
                hands_[m.get_from()].push_back(deck_.back());
                deck_.pop_back();
            }

            if ((playing_card.rank() == five) && (hint_tokens_ < 8)) {
                hint_tokens_++; // get a hint back if a pile is completed
            }
        } else { // FAILED PLAY
            discards_.push_back(playing_card);
            auto it = std::find(hands_[m.get_from()].begin(), hands_[m.get_from()].end(), 
                                playing_card);
            hands_[m.get_from()].erase(it);
            if (!(deck_.empty())) {
                if (log) deck_.back().str();
                hands_[m.get_from()].push_back(deck_.back());
                deck_.pop_back();
            }

            lives_--;
        }
    } else { // A hint was given
        hint_tokens_--;
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
    for (int p = 0; p < num_players_; p++) {
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