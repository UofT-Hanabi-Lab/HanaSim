#include "../include/belief_state.h"

BeliefState::BeliefState(int num_players, int bot_id) : 
    State(num_players), bot_id_(bot_id){
    initialize_uniform_belief();
}

// 1. Uniform belief: all hands except the bot’s are known, so belief is based 
// on remaining cards
void BeliefState::initialize_uniform_belief() {

    // possible_cards are cards that could be on this player's private hand
    std::multiset<Card> possible_cards(deck_.begin(), deck_.end());

    for (int p = 0; p < num_players_; ++p) {
        if (p == bot_id_) continue; // bot itself 
        for (const Card& c : hands_[p]) {
            // if the card is on other player's hand, erase that from possible cards.
            possible_cards.erase(possible_cards.find(c));
        }
    }

    belief_hand_.clear();
    
    /*example of the dist:
        dist = {(<Red,1>:0.2), (<Blue,2>:0.1), ...}
    */
    std::map<std::pair<Color, Rank>, double> dist;
    for (const auto& c : possible_cards) {
        dist[{c.color(), c.rank()}] += 1.0;
    }
    // Normalize
    double total = static_cast<double>(possible_cards.size());
    for (auto& p : dist){
        p.second /= total;
    } 

    /* example of belief_hand_:
        all slots in the hand are the same.
        belief_hand_ = [dist, dist, ...(number of cards per hand)]
    */
    for (int i = 0; i < cards_per_hand_; ++i) {
        belief_hand_.push_back(dist);
    }
}

// 2. Sample one plausible hand consistent with belief
std::vector<Card> BeliefState::sample_hand() {
    std::vector<Card> sampled; // the vector of a hand of cards
    
    // To make sure the sampling is dependent in each round(number of cards in a hand), 
    // avoiding picking repeatatively on one card.
    std::multiset<Card> available(deck_.begin(), deck_.end()); // all possible cards in private hand. 

    for (int p = 0; p < num_players_; ++p) {
        if (p == bot_id_) continue;// also TODO: check if hands_[0] is the acting agent
        for (const Card& c : hands_[p])
            available.erase(available.find(c));
    }

    for (const auto& slot : belief_hand_) {
        double r = (double)rand() / RAND_MAX;
        double acc = 0.0;
        // the logic for simulate on slot is that:
            // we have a spectrum of prob distribution of cards. 
            // r is the cursor that points to some where in the spectrum. 
            // If a card has larger prob, it will take up more space on the 
            // spectrum and thus easier to be pointed by the cursor.
            // intuitively, the cursor is the r and acc is the spectrum creator.
        for (const auto& [card, prob] : slot) {
            acc += prob;
            if (r <= acc) {
                sampled.push_back(Card(card.first, card.second));
                available.erase(available.find(Card(card.first, card.second)));
                break;
            }
        }
    }

    return sampled;
}

// 3. Create a full sampled state from belief
State BeliefState::sample_belief_state() {
    State sampled_state = *this; // clone the public info 
    std::vector<Card> hand0 = sample_hand();
    sampled_state.get_hands()[bot_id_] = hand0; // overwrite bot's hand
    return sampled_state;
}

// Updates the belief based on the observation (e.g., hint or play).
void BeliefState::update_belief(State s, move m){
    switch(m.get_type()){
        case COL_HINT:
        case RANK_HINT:{
            if (m.get_to() != bot_id_) break;// only update when private hand is hinted

            std::vector<int> hinted_indices = m.get_card_index();
            Rank hinted_rank = m.get_rank();

            for (int i = 0; i < cards_per_hand_; ++i) {
                auto& dist = belief_hand_[i];
                std::map<std::pair<Color, Rank>, double> updated_dist;

                //check if i is the slot that was hinted by iterating through the hinted_indices list.
                bool is_slot_hinted = std::find(hinted_indices.begin(), hinted_indices.end(), i) != hinted_indices.end();
                
                for (const auto& [card, prob] : dist) {
                    bool matches = (m.get_type() == COL_HINT)
                        ? (card.first == m.hint_color)
                        : (card.second == m.hint_rank);

                    // Keep the card only if:
                    // - slot is hinted and card matches the hint
                    // - slot is not hinted and card does NOT match the hint
                    if ((is_slot_hinted && matches) || (!is_slot_hinted && !matches)) {
                        updated_dist[card] = prob;
                    }
                }

                // Normalize
                double sum = 0.0;
                for (auto& [_, p] : updated_dist) sum += p;
                if (sum > 0) {
                    for (auto& [_, p] : updated_dist) p /= sum;
                }

                dist = updated_dist;
            }
            break;
        }
        case PLAY:
        case DISCARD:{
            if (m.get_from() != bot_id_) break;

            int removed_idx = m.get_card_index();

            // Remove the card slot at index
            belief_hand_.erase(belief_hand_.begin() + removed_idx);

            std::multiset<Card> get_possible_remaining_deck(s, int bot_id_);

            // Create a new belief distribution for the newly drawn card
            std::map<std::pair<Color, Rank>, double> new_dist;
            for (const auto& c : possible_cards) {
                new_dist[{c.color(), c.rank()}] += 1.0;
            }

            double total = static_cast<double>(possible_cards.size());
            for (auto& p : new_dist) p.second /= total;

            belief_hand_.push_back(new_dist);
            break;
        }

        default:
            break;
    }
}

/*helper functions*/
std::multiset<Card> get_possible_remaining_deck(const State& s, int bot_id) {
    std::multiset<Card> full_deck = {
        Card(red, one), Card(red, one), Card(red, one), Card(red, two), Card(red, two), Card(red, three), Card(red, three), Card(red, four), Card(red, four), Card(red, five), 
        Card(blue, one), Card(blue, one), Card(blue, one), Card(blue, two), Card(blue, two), Card(blue, three), Card(blue, three), Card(blue, four), Card(blue, four), Card(blue, five),
        Card(yellow, one), Card(yellow, one), Card(yellow, one), Card(yellow, two), Card(yellow, two), Card(yellow, three), Card(yellow, three), Card(yellow, four), Card(yellow, four), Card(yellow, five),
        Card(green, one), Card(green, one), Card(green, one), Card(green, two), Card(green, two), Card(green, three), Card(green, three), Card(green, four), Card(green, four), Card(green, five),
        Card(white, one), Card(white, one), Card(white, one), Card(white, two), Card(white, two), Card(white, three), Card(white, three), Card(white, four), Card(white, four), Card(white, five)
    };

    // Remove visible cards in other players' hands
    const auto& hands = s.get_hands();
    for (int p = 0; p < hands.size(); ++p) {
        if (p == bot_id) continue;
        for (const Card& c : hands[p]) {
            full_deck.erase(full_deck.find(c));
        }
    }

    // Remove discarded cards
    for (const Card& c : s.get_discards()) {
        full_deck.erase(full_deck.find(c));
    }

    // Remove cards from completed piles
    std::vector<int> piles = s.get_piles();
    for (int col = 0; col < 5; ++col) {
        for (int r = 1; r <= piles[col]; ++r) {
            full_deck.erase(full_deck.find(Card(static_cast<Color>(col), static_cast<Rank>(r))));
        }
    }

    // NOTE: We do not remove own-hand cards (unknown to player)
    return full_deck;
}
