#include <iostream>
#include <stdio.h>
#include <algorithm>
#include <random>
#include <map>

#include "../include/state.h"

// Forward declarations of encoding functions
int BitsPerCard();
int HandsSectionLength(int num_players, int cards_per_hand);
int BoardSectionLength();
int DiscardSectionLength();
void EncodeHands(const State& state, int player_id, std::vector<int>& encoding, int& offset);
void EncodeBoard(const State& state, std::vector<int>& encoding, int& offset);
void EncodeDiscards(const State& state, std::vector<int>& encoding, int& offset);

State::State(int num_players) {
    // Initialize the deck, 50 cards
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
    // set Blue tokens and lives
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

    // Initialize card knowledge
    possible_colors_ = std::vector<std::vector<std::vector<bool>>>(
        num_players_,
        std::vector<std::vector<bool>>(
            cards_per_hand_,
            std::vector<bool>(6, true)  // 6 colors (including invalid_color)
        )
    );
    possible_ranks_ = std::vector<std::vector<std::vector<bool>>>(
        num_players_,
        std::vector<std::vector<bool>>(
            cards_per_hand_,
            std::vector<bool>(6, true)  // 6 ranks (including invalid_rank)
        )
    );

    // Set index 0 to false since colors and ranks start from 1
    for (int p = 0; p < num_players_; p++) {
        for (int c = 0; c < cards_per_hand_; c++) {
            possible_colors_[p][c][0] = false;
            possible_ranks_[p][c][0] = false;
        }
    }
}

std::vector<move> State::get_legal_moves(int id) const{
    std::vector<move> moves;
    moves = {};
    // Enumerate all legal moves using brute-force search.
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
                if (auto search = col_to_indices.find(c.color()); search == col_to_indices.end()) { // Not exist then create
                    col_to_indices.insert({c.color(), {j}});
                } else {
                    col_to_indices.at(c.color()).push_back(j);
                }
                ranks.insert(c.rank());  // Similar for rank
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

void State::update_card_knowledge(const move& m) {
    if (m.get_type() == COL_HINT) {
        Color hinted_color = m.get_color();
        int target_player = m.get_to();
        std::vector<int> hinted_indices = m.get_card_indices();
        
        // For each card in the target player's hand
        for (int i = 0; i < hands_[target_player].size(); i++) {
            // If this card was hinted
            if (std::find(hinted_indices.begin(), hinted_indices.end(), i) != hinted_indices.end()) {
                // This card must be of the hinted color
                for (int c = 1; c <= 5; c++) {  // Skip invalid_color
                    possible_colors_[target_player][i][c] = (c == static_cast<int>(hinted_color));
                }
            } else {
                // This card cannot be of the hinted color
                possible_colors_[target_player][i][static_cast<int>(hinted_color)] = false;
            }
        }
    }
    else if (m.get_type() == RANK_HINT) {
        Rank hinted_rank = m.get_rank();
        int target_player = m.get_to();
        std::vector<int> hinted_indices = m.get_card_indices();
        
        // For each card in the target player's hand
        for (int i = 0; i < hands_[target_player].size(); i++) {
            // If this card was hinted
            if (std::find(hinted_indices.begin(), hinted_indices.end(), i) != hinted_indices.end()) {
                // This card must be of the hinted rank
                for (int r = 1; r <= 5; r++) {  // Skip invalid_rank
                    possible_ranks_[target_player][i][r] = (r == static_cast<int>(hinted_rank));
                }
            } else {
                // This card cannot be of the hinted rank
                possible_ranks_[target_player][i][static_cast<int>(hinted_rank)] = false;
            }
        }
    }
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
            
            // Initialize knowledge for the new card
            int new_card_index = hands_[m.get_from()].size() - 1;
            possible_colors_[m.get_from()][new_card_index] = std::vector<bool>(6, true);
            possible_ranks_[m.get_from()][new_card_index] = std::vector<bool>(6, true);
            // Set index 0 to false since colors and ranks start from 1
            possible_colors_[m.get_from()][new_card_index][0] = false;
            possible_ranks_[m.get_from()][new_card_index][0] = false;
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
            
            // Initialize knowledge for the new card
            int new_card_index = hands_[m.get_from()].size() - 1;
            possible_colors_[m.get_from()][new_card_index] = std::vector<bool>(6, true);
            possible_ranks_[m.get_from()][new_card_index] = std::vector<bool>(6, true);
            // Set index 0 to false since colors and ranks start from 1
            possible_colors_[m.get_from()][new_card_index][0] = false;
            possible_ranks_[m.get_from()][new_card_index][0] = false;
        }
    } else if (m.get_type() == COL_HINT || m.get_type() == RANK_HINT){ // A hint was given
        hint_tokens_--;
        update_card_knowledge(m);  // Update card knowledge based on the hint
        if (log) std::cout << std::endl;
    } else{
        if (log) std::cout << "Invalid move type:"<<m.get_type()<<std::endl;
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
    
    // Reset card knowledge
    possible_colors_ = std::vector<std::vector<std::vector<bool>>>(
        num_players_,
        std::vector<std::vector<bool>>(
            cards_per_hand_,
            std::vector<bool>(6, true)
        )
    );
    possible_ranks_ = std::vector<std::vector<std::vector<bool>>>(
        num_players_,
        std::vector<std::vector<bool>>(
            cards_per_hand_,
            std::vector<bool>(6, true)
        )
    );

    // Set index 0 to false since colors and ranks start from 1
    for (int p = 0; p < num_players_; p++) {
        for (int c = 0; c < cards_per_hand_; c++) {
            possible_colors_[p][c][0] = false;
            possible_ranks_[p][c][0] = false;
        }
    }
}

static const std::vector<std::string> color_names = {"empty", "red", "white", "yellow", "green", "blue"};
std::string color_to_string(Color color) {
    return color_names[static_cast<int>(color)];
}


// Encode the move object to std::tuple
std::tuple<int, int, int, int, std::vector<int>, int, int> encode_move(const move &m) {
    return std::make_tuple(
        static_cast<int>(m.get_type()),
        m.get_to(),
        m.get_from(),
        m.get_card_index(),
        m.get_card_indices(),
        static_cast<int>(m.get_color()),
        static_cast<int>(m.get_rank())
    );
}


Observation State::get_observation(int player_id) const {
    Observation obs;

    obs.hands.clear();
    obs.hands.resize(hands_.size());
    for (size_t i = 0; i < hands_.size(); i++){
        for (const Card& card : hands_[i]) {
            obs.hands[i].push_back({color_to_string(card.color()), card.rank()});
        }
    }
    
    // Add card knowledge to observation
    obs.possible_colors = possible_colors_;
    obs.possible_ranks = possible_ranks_;
    
    for (size_t i = 1; i < piles_.size(); ++i) {
        obs.fireworks[color_names[i]] = piles_[i];
    }

    obs.hint_tokens = hint_tokens_;
    obs.lives_remaining = lives_;
    obs.deck_size = deck_.size();

    for (const Card& card : discards_) {
        obs.discards.push_back({color_to_string(card.color()), card.rank()});
    }

    std::vector<move> legal_moves = get_legal_moves(player_id);
    for (const move& m : legal_moves) {
        obs.legal_actions.push_back(encode_move(m));
    }

    obs.current_player_id = player_id;

    // Create vector encoding
    int total_length = HandsSectionLength(num_players_, cards_per_hand_) +
                      BoardSectionLength() +
                      DiscardSectionLength();
    obs.vector_encoding = std::vector<int>(total_length, 0);
    
    int offset = 0;
    EncodeHands(*this, player_id, obs.vector_encoding, offset);
    EncodeBoard(*this, obs.vector_encoding, offset);
    EncodeDiscards(*this, obs.vector_encoding, offset);

    return obs;
}

// Helper functions for bit vector encoding
int BitsPerCard() {
    return 5 * 5;  // 5 colors * 5 ranks
}

int HandsSectionLength(int num_players, int cards_per_hand) {
    return (num_players - 1) * cards_per_hand * BitsPerCard() +  // Other players' hands
           cards_per_hand * (5 + 5);  // Current player's card knowledge (possible colors and ranks)
}

int BoardSectionLength() {
    return 50 +  // deck size (thermometer encoding)
           5 * 5 +  // fireworks (one-hot per color)
           8 +  // info tokens (thermometer)
           3;   // life tokens (thermometer)
}

int DiscardSectionLength() {
    return 50;  // max deck size
}

// Encode hands section
void EncodeHands(const State& state, int player_id, std::vector<int>& encoding, int& offset) {
    // Encode other players' hands
    const auto& hands = state.get_hands();
    for (int p = 1; p < hands.size(); p++) {
        int actual_p = (player_id + p) % hands.size();  // Relative to current player
        for (const Card& card : hands[actual_p]) {
            encoding[offset + (card.color() - 1) * 5 + (card.rank() - 1)] = 1;
            offset += BitsPerCard();
        }
    }

    // Encode current player's card knowledge
    const auto& possible_colors = state.get_possible_colors();
    const auto& possible_ranks = state.get_possible_ranks();
    for (int c = 0; c < hands[player_id].size(); c++) {
        // Encode possible colors
        for (int color = 1; color <= 5; color++) {
            if (possible_colors[player_id][c][color]) {
                encoding[offset + color - 1] = 1;
            }
        }
        offset += 5;

        // Encode possible ranks
        for (int rank = 1; rank <= 5; rank++) {
            if (possible_ranks[player_id][c][rank]) {
                encoding[offset + rank - 1] = 1;
            }
        }
        offset += 5;
    }
}

// Encode board section
void EncodeBoard(const State& state, std::vector<int>& encoding, int& offset) {
    // Encode deck size (thermometer)
    const auto& deck = state.get_deck();
    for (int i = 0; i < deck.size(); i++) {
        encoding[offset + i] = 1;
    }
    offset += 50;

    // Encode fireworks
    const auto& piles = state.get_piles();
    for (int c = 1; c <= 5; c++) {
        if (piles[c] > 0) {
            encoding[offset + (c-1)*5 + piles[c] - 1] = 1;
        }
    }
    offset += 25;

    // Encode info tokens (thermometer)
    for (int i = 0; i < state.get_num_hints(); i++) {
        encoding[offset + i] = 1;
    }
    offset += 8;

    // Encode life tokens (thermometer)
    for (int i = 0; i < state.get_num_lives(); i++) {
        encoding[offset + i] = 1;
    }
    offset += 3;
}

// Encode discard section
void EncodeDiscards(const State& state, std::vector<int>& encoding, int& offset) {
    std::vector<std::vector<int>> discard_counts(5, std::vector<int>(5, 0));
    const auto& discards = state.get_discards();
    for (const Card& card : discards) {
        discard_counts[card.color()-1][card.rank()-1]++;
    }

    for (int c = 0; c < 5; c++) {
        for (int r = 0; r < 5; r++) {
            for (int i = 0; i < discard_counts[c][r]; i++) {
                encoding[offset + i] = 1;
            }
            offset += 2;  // Max 2 cards of each type
        }
    }
}