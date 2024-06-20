#include <iostream>
#include <stdio.h>
#include <algorithm>
#include <random>

#include "../include/state.h"

State::State(int num_players, int cards_per_hand) {
    num_players_ = num_players;
    cards_per_hand_ = cards_per_hand;
    hint_tokens_ = 8;
    lives_ = 3;
    deck_ = {};
    for (int col = red; col != white; col++) {
        for (int rank = one; rank <= five; rank++) {
            int num_rank;
            switch(rank) {
                case one:
                    num_rank = 3;
                    break;
                case five:
                    num_rank = 1;
                    break;
                default:
                    num_rank = 2;
            }
            for (int i = 0; i < num_rank; i++) {
                deck_.push_back(Card(static_cast<Color>(col), static_cast<Rank>(rank)));
                
            }
        }
    }
    // Shuffling the deck
    std::random_device rd;
    std::default_random_engine gen(rd());
    std::shuffle(deck_.begin(), deck_.end(), gen);
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
    piles_ = {0, 0, 0, 0, 0}; // all piles are empty
}

void State::transition(move m) {
    if (m.get_type() == DISCARD) {
        Card discard = hands_[m.get_from()][m.get_card_index()];
        discards_.push_back(discard);
        auto it = std::find(hands_[m.get_from()].begin(), hands_[m.get_from()].end(), 
                            discard);
        hands_[m.get_from()].erase(it);
        if (!(deck_.empty())) {
            hands_[m.get_from()].push_back(deck_.back());
            deck_.pop_back();
        }
        hint_tokens_++; // if there were 8 hint tokens, a discard shoudn't have even been made
    } else if (m.get_type() == PLAY) {
        Card playing_card = hands_[m.get_from()][m.get_card_index()];
        int top_rank = piles_[playing_card.color() - 1];
        if (playing_card.rank() == top_rank + 1) { // SUCCESSFUL PLAY
            piles_[playing_card.color() - 1] += 1;
            auto it = std::find(hands_[m.get_from()].begin(), hands_[m.get_from()].end(), 
                                playing_card);
            hands_[m.get_from()].erase(it);
            if (!(deck_.empty())) {
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

