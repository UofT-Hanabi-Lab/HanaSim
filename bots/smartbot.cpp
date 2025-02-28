#include <cstring>
#include <iostream>
#include "include/smartbot.h"


using namespace SmartBotInternal;


/*************************** CARD KNOWLEDGE IMPLEMENTATION ***************************/

smart_cardknowledge::smart_cardknowledge(smartbot *bot) {
    // At the beginning, we know nothing, so initialize values accordingly.
    bot_ = bot;
    possibilities_ = -1;
    color_ = -2; // -2 means we dont't know the color because we haven't even attempted to compute/infer it. -1 means we have attempted to infer it, but don't have enough knowledge.
    rank_ = -2; // ^
    memset(cant_be_, '\0', sizeof cant_be_);  // Initialize all values to false. All cards could be anything
    playable_ = valuable_ = worthless_ = MAYBE;
    prob_playable_ = prob_valuable_ = prob_worthless_ = -1.0;
}


bool smart_cardknowledge::must_be(Color color) {
    return (this->color_ == color);
}


bool smart_cardknowledge::must_be(Rank rank) {
    return (this->rank_ == rank);
}


bool smart_cardknowledge::cannot_be(Card card)  {
    return cant_be_[card.color()][card.rank()];
}


bool smart_cardknowledge::cannot_be(Color color) {
    if (color_ != -1) {  // Case 1: the color is already known
        return (color_ != color);
    }
    for (int v = 1; v < 6; v++) {  // Case 2: the color is still unknown
        if (!cant_be_[color][v]) {
            // At least one valid rank exists for this color
            return false;
        }
    }
    return true;
}


bool smart_cardknowledge::cannot_be(Rank rank) {
    if (this->rank_ != -1) {  // Case 1: the rank is already known
        return (this->rank_ != rank);
    }
    for (int k = 1; k < 6; k++) {  // Case 2: the rank is still unknown
        if (!cant_be_[k][rank]) {
            // At least one valid color exists for this rank
            return false;
        }
    }
    return true;
}


void smart_cardknowledge::befuddle_by_discard() {
    /* Discard could make non-valuable cards valauble and non-worthless cards worthless */
    if (worthless_ != YES) { // only if it wasn't worthless before. If it was, it continues to be worthless (and not valuable)
        valuable_ = worthless_ = MAYBE;
        prob_valuable_ = prob_worthless_ = -1.0f;
    }
}


void smart_cardknowledge::befuddle_by_play(bool success) {
    if (success) { // if successfully played, some card might lose/gain playability
        playable_ = MAYBE;
        prob_playable_ = -1.0f;
    } else { // if play failed, some cards might lose/gain value
        valuable_ = MAYBE;
        prob_valuable_ = -1.0f;
    }
    if (worthless_ != YES) { // same logic as befuddle_by_discard
        worthless_ = MAYBE;
        prob_worthless_ = -1.0f;
    }
}


void smart_cardknowledge::set_must_be(Color color) {
    for (int k = 1; k < 6; k++) {
        for (int r = 0; r < 6; r++) {
            if (k != color) cant_be_[k][r] = true;
        }
    }
    color_ = color;

    // Haven't re-computed anything else, so set to -1 (except for rank, which should be -2)
    possibilities_ = -1;
    if (rank_ == -1) rank_ = -2;
    if (playable_ == MAYBE) prob_playable_ = -1.0;
    if (valuable_ == MAYBE) prob_valuable_ = -1.0;
    if (worthless_ == MAYBE) prob_worthless_ = -1.0;
}


void smart_cardknowledge::set_must_be(Rank rank) {
    for (int v = 1; v < 6; v++) {
        for (int k = 1; k < 6; k++) {
            if (v != rank) cant_be_[k][v] = true;
        }
    }
    rank_ = rank;

    possibilities_ = -1;
    if (color_ == -1) color_= -2;
    if (playable_ == MAYBE) prob_playable_ = -1.0;
    if (valuable_ == MAYBE) prob_valuable_ = -1.0;
    if (worthless_ == MAYBE) prob_worthless_ = -1.0;
}


void smart_cardknowledge::set_must_be(Card card) {
    for (int v = 1; v < 6; v++) {
        for (int k = 1; k < 6; k++) {
            cant_be_[k][v] = !(k == card.color() && v == card.rank());
        }
    }
    possibilities_ = 1; // we know there's only one possibility
    color_ = card.color();
    rank_ = card.rank();

    if (playable_ == MAYBE) prob_playable_ = -1.0;
    if (valuable_ == MAYBE) prob_valuable_ = -1.0;
    if (worthless_ == MAYBE) prob_worthless_ = -1.0;
}


void smart_cardknowledge::set_cannot_be(Color color) {
    for (int v = 1; v < 6; v++) {
        cant_be_[color][v] = true;
    }

    possibilities_ = -1;
    if (color_ == -1) color_= -2;
    if (rank_ == -1) rank_ = -2;
    if (playable_ == MAYBE) prob_playable_ = -1.0;
    if (valuable_ == MAYBE) prob_valuable_ = -1.0;
    if (worthless_ == MAYBE) prob_worthless_ = -1.0;
}


void smart_cardknowledge::set_cannot_be(Rank rank) {
    for (int k = 0; k < 6; k++) {
        cant_be_[k][rank] = true;
    }

    possibilities_ = -1;
    if (color_ == -1) color_= -2;
    if (rank_ == -1) rank_ = -2;
    if (playable_ == MAYBE) prob_playable_ = -1.0;
    if (valuable_ == MAYBE) prob_valuable_ = -1.0;
    if (worthless_ == MAYBE) prob_worthless_ = -1.0;
}


/*
Set whether a card is playable
Inference made: a playable card cannot be worthless
*/
void smart_cardknowledge::set_is_playable(bool playable, State s) {
    for (int k = 1; k < 6; k++) {
        for (int r = 1; r < 6; r++) {
            if (cant_be_[k][r]) continue;
            if (bot_->is_playable(s, Card(Color(k),Rank(r))) != playable) { // so it can't be this color or rank since they differ in playability
                cant_be_[k][r] = true;
            }
        }
    }

    playable_ = (playable ? YES : NO);
    prob_playable_ = (playable ? 1.0 : 0.0);

    possibilities_ = -1;
    if (color_ == -1) color_ = -2;
    if (rank_ == -1) rank_ = -2;
    
    if (valuable_ == MAYBE) prob_valuable_ = -1.0;
    if (worthless_ == MAYBE) prob_worthless_ = -1.0;

    if (playable) {
        worthless_ = NO;
        prob_worthless_ = 0.0;
    }
}


/*
Set whether a card is valuable
Inference made: a valuable card cannot be worthless
*/
void smart_cardknowledge::set_is_valuable(bool valuable, State s) {
    for (int k = 1; k < 6; k++) {
        for (int r = 1; r < 6; r++) {
            if (cant_be_[k][r]) {
                continue;
            }
            if (bot_->is_valuable(s, Card(Color(k),Rank(r))) != valuable) { // same logic as above func
                cant_be_[k][r] = true;
            }
        }
    }

    valuable_ = (valuable ? YES : NO);
    prob_valuable_ = (valuable ? 1.0 : 0.0);

    possibilities_ = -1;
    if (color_ == -1) color_ = -2;
    if (rank_ == -1) rank_ = -2;
    if (playable_ == MAYBE) prob_playable_ = -1.0;
    if (worthless_ == MAYBE) prob_worthless_ = -1.0;

    if (valuable) {
        worthless_ = NO;
        prob_worthless_ = 0.0;
    }
}


/*
Set whether a card is worthless
Inference made: a worthless card cannot be playable nor valuable
*/
void smart_cardknowledge::set_is_worthless(bool worthless, State s) {
    for (int k = 1; k < 6; k++) {
        for (int r = 1; r < 6; r++) {
            if (cant_be_[k][r]) {
                continue;
            }
            if (bot_->is_worthless(s, Card(Color(k),Rank(r))) != worthless) { // same logic as above func
                cant_be_[k][r] = true;
            }
        }
    }

    worthless_ = (worthless ? YES : NO);
    prob_worthless_ = (worthless ? 1.0 : 0.0);

    possibilities_ = -1;
    if (color_ == -1) color_ = -2;
    if (rank_ == -1) rank_ = -2;
    if (playable_ == MAYBE) prob_playable_ = -1.0;
    if (valuable_ == MAYBE) prob_valuable_ = -1.0;
    
    if (worthless) {
        playable_ = valuable_ = NO;
        prob_playable_ = prob_valuable_ = 0.0;
    }
}


/*
Attempts to compute a cards identity (color and rank)
given the current information available.
If a color and/or rank are successfully identified, set them to the computed values,
otherwise set them to -1. */
void smart_cardknowledge::compute_identity() {
    if (color_ != -2 && rank_ != -2) return; // we've already computed so no need to re-compute

    int color = -2;
    int rank = -2;
    for (int k = 1; k < 6; k++) {
        for (int r = 1; r < 6; r++) {
            if (cant_be_[k][r]) {
                continue;
            }
            color = (color == -2 || color == k) ? k : -1; // will be -1 if color has more than one possibility
            rank = (rank == -2 || rank == r) ? r : -1; // ^
        }
    }
    if (color == -2) {
        color = -1;
    }
    if (rank == -2) {
        rank = -1;
    }
    color_ = color;
    rank_ = rank;
}


/*
Count the number of possible valid (color, rank) configurations for the card.
*/
void smart_cardknowledge::compute_possibilities() {
    if (possibilities_ != -1) { // no need to re-compute
        return;
    }
    int poss = 0;
    for (int k = 1; k < 6; k++) {
        for (int r = 1; r < 6; r++) {
            poss += !cant_be_[k][r];
        }
    }
    possibilities_ = poss;
}


/*
Given the current state, compute the probability that a card is playable,
and set the playable_ attribute accordingly.
*/
void smart_cardknowledge::compute_playable(State s)
{
    if (prob_playable_ != -1.0f) {
        return;
    }
    int total_count = 0;
    int yes_count = 0;
    for (int k = 1; k < 6; k++) {
        for (int r = 1; r < 6; r++) {
            if (cant_be_[k][r]) {
                continue;
            }
            total_count += 1;
            yes_count += (bot_->is_playable(s, Card(Color(k), Rank(r))));
        }
    }
    if (total_count < 1) { // only happens if everything in cant_be is true
      prob_playable_ = 0.5;
      playable_ = MAYBE;
      return;
    }
    prob_playable_ = (float)yes_count / total_count;
    playable_ = (yes_count == total_count) ? YES : (yes_count != 0) ? MAYBE : NO;
}


/*
Given the current state, compute the probability that a card is valuable,
and set the valuable_ attribute accordingly.
*/
void smart_cardknowledge::compute_valuable(State s)
{
    if (prob_valuable_ != -1.0f) { // no need to re-compute
        return;
    }
    int total_count = 0;
    int yes_count = 0;
    for (int k = 1; k < 6; k++) {
        for (int r = 1; r < 6; r++) {
            if (cant_be_[k][r]) {
                continue;
            }
            total_count += 1;
            yes_count += (bot_->is_valuable(s, Card(Color(k), Rank(r))));
        }
    }
    if (total_count < 1) {
      prob_valuable_ = 0.5;
      valuable_ = MAYBE;
      return;
    }
    prob_valuable_ = (float)yes_count / total_count;
    valuable_ = (yes_count == total_count) ? YES : (yes_count != 0) ? MAYBE : NO;
}


/*
Given the current state, compute the probability that a card is worthless,
and set the worthless_ attribute accordingly.
*/
void smart_cardknowledge::compute_worthless(State s)
{
    if (prob_worthless_ != -1.0f) { // no need to re-compute
        return;
    }
    int total_count = 0;
    int yes_count = 0;
    for (int k = 1; k < 6; k++) {
        for (int r = 1; r < 6; r++) {
            if (cant_be_[k][r]) {
                continue;
            }
            total_count += 1;
            yes_count += (bot_->is_worthless(s, Card(Color(k), Rank(r))));
        }
    }
    if (total_count < 1) {
      prob_worthless_ = 0.5;
      worthless_ = MAYBE;
      return;
    }
    prob_worthless_ = (float)yes_count / total_count;
    worthless_ = (yes_count == total_count) ? YES : (yes_count != 0) ? MAYBE : NO;
}


/*
Eliminate impossible card possibilities based on the played/discarded cards
and possibly other player's hands (based on the value of use_eyesight)
*/
void smart_cardknowledge::update(bool use_eyesight) {
    if (!known()) {
        bool recompute = false;
        for (int k = 1; k < 6; k++) {
            for (int r = 1; r < 6; r++) {
                if (cant_be_[k][r]) {
                    continue;
                }
                int total = (r == 1 ? 3 : (r == 5 ? 1 : 2));
                int played = bot_->played_count_[k][r];
                int held = (use_eyesight ? bot_->eyesight_count_[k][r] : bot_->located_count_[k][r]);
                if (played + held >= total) {
                    cant_be_[k][r] = true;
                    recompute = true;
                }
            }
        }
        if (recompute) { // need to re-compute everything cuz of new inferred knowledge
            possibilities_ = -1;
            color_ = -2;
            rank_ = -2;
            playable_ = valuable_ = worthless_ = MAYBE;
            prob_playable_ = prob_valuable_ = prob_worthless_ = -1.0f;
        }
    }
}


/*
Checks whether a card would be playable if it was of the provided input rank
*/
bool smart_cardknowledge::could_be_playable(int rank, State s) {
    if (rank < 1 || rank > 5 || cannot_be(Rank(rank))) {
        return false;
    }
    if (playable(s) != MAYBE) { // if we're certain of playability, why are we calling this function?
        return false;
    }
    smart_cardknowledge new_knowl = *this;
    new_knowl.set_must_be(Rank(rank));
    return (new_knowl.playable(s) != NO);
}


/*
Checks whether a card would be valuable if it was of the provided input rank
*/
bool smart_cardknowledge::could_be_valuable(int rank, State s) {
    if (rank < 1 || rank > 5 || cannot_be(Rank(rank))) {
        return false;
    }
    if (valuable(s) != MAYBE) {
        return false;
    }
    smart_cardknowledge new_knowl = *this;
    new_knowl.set_must_be(Rank(rank));
    return (new_knowl.valuable(s) != NO);
}


/*************************** SMARTBOT IMPLEMENTATION ***************************/

smartbot::smartbot(int id, int n_players) {
    id_ = id;
    num_cards_ = (n_players <= 3) ? 5 : 4;
    hand_knowledge_.resize(n_players);
    for (int i = 0; i < n_players; i++) {
        hand_knowledge_[i].resize(num_cards_, smart_cardknowledge(this));
    }
    memset(played_count_, '\0', sizeof played_count_);
}


/*
A card is playable if it can be played on the current pile of its color.
*/
bool smartbot::is_playable(State s, Card card) { return (card.rank() == s.get_piles()[card.color()] + 1); }


/*
A card is valuable if it is not worthless and is the last copy of its kind.
*/
bool smartbot::is_valuable(State s, Card card) {
    int total = ((card.rank() == 1) ? 3 : ((card.rank() == 5) ? 1 : 2));
    if (played_count_[card.color()][card.rank()] != total - 1) {
        return false;
    }
    return !(is_worthless(s, card));
}


/*
A card is worthless if can no longer affect the score of the game.
*/
bool smartbot::is_worthless(State s, Card card) {
    int v = card.rank();
    if (v < s.get_piles()[card.color()] + 1) {
        return true;
    }
    while (v > s.get_piles()[card.color()] + 1) {
        v--;
        int total = ((v == 1) ? 3 : ((v == 5) ? 1 : 2));
        if (played_count_[card.color()][v] == total) {
            return true;
        }
    }
    return false;
}


/*
Shift hand knowledge whenever a card is played or discarded.
Adjust the shift based on whether a new card was also drawn.
*/
void smartbot::shift_knowledge(int p_index, int c_index, bool draw) {
    for (int i = c_index; i + 1 < hand_knowledge_[p_index].size(); i++) {
        hand_knowledge_[p_index][i] = hand_knowledge_[p_index][i + 1];
    }
    if (draw) {
        hand_knowledge_[p_index].back() = smart_cardknowledge(this);
    } else {
        hand_knowledge_[p_index].pop_back();
    }
}


/*
Update the number of cards fully known to each bot.
Implement a different counting strategy for the bot's own cards
and the other players' cards.
*/
void smartbot::update_eyesight_count(State s) {
    memset(eyesight_count_, '\0', sizeof eyesight_count_);
    for (int p = 0; p < hand_knowledge_.size(); p++) {
        if (p == id_) { // own cards: only increment array for fully known cards
            for (int i = 0; i < s.get_hands()[id_].size(); i++) {
                if (hand_knowledge_[p][i].known()) {
                    eyesight_count_[hand_knowledge_[p][i].color()][hand_knowledge_[p][i].rank()]++;
                }
            }
        } else { // other players' cards: we can use eyesight for other people's cards
            std::vector<Card> hand = s.get_hands()[p];
            for (int i = 0; i < hand.size(); i++) {
                eyesight_count_[hand[i].color()][hand[i].rank()]++;
            }
        }
    }
}


bool smartbot::update_located_count() { // cards that everyone knows the location of (in people's hands)
    int new_count[6][6] = {};
    for (int p = 0; p < hand_knowledge_.size(); p++) {
        for (int i = 0; i < hand_knowledge_[p].size(); i++) {
            if (hand_knowledge_[p][i].known()) {
                new_count[hand_knowledge_[p][i].color()][hand_knowledge_[p][i].rank()] += 1;
            }
        }
    }
    if (std::memcmp(located_count_, new_count, sizeof new_count) != 0) {
        std::memcpy(located_count_, new_count, sizeof new_count);
        return true;
    }
    return false;
}


/*
Determines the next best card to be discarded based on the
current game state and their hand.
*/
int smartbot::next_discard_index(State s, int player_index) {
    double best_f = 0;
    int best_i = -1;
    for (int i = 0; i < hand_knowledge_[player_index].size(); i++) {
        if (hand_knowledge_[player_index][i].playable(s) == YES) {
            return -1;
        }
        if (hand_knowledge_[player_index][i].worthless(s) == YES) { // can discard with no risk
            return -1;
        }
        if (hand_knowledge_[player_index][i].valuable(s) == YES) {
            continue;
        }
        
        double f = 100 + hand_knowledge_[player_index][i].prob_worthless(s); // want to discard the card with highest prob_worthless
        if (f > best_f) {
            best_f = f;
            best_i = i;
        }
    }
    return best_i;
}


void smartbot::no_warning_given(int from, State s) {
    if (s.get_deck().size() == 0 || s.get_num_hints() == 0) {
        return;
    }
    int disc_ind = next_discard_index(s, (from + 1) % hand_knowledge_.size());
    if (disc_ind != -1) {
        hand_knowledge_[(from + 1) % hand_knowledge_.size()][disc_ind].set_is_valuable(false, s); // so my next discard definitely isn't valuable (otherwise, I would've been warned)
    }
}


/*
Update the knowledge for every player before any more is made.
*/
void smartbot::observe_before_move(State s) {
    memset(located_count_, '\0', sizeof located_count_);
    update_located_count();
    // similar to smartbot's observe_before_move
    do {
        for (int p = 0; p < hand_knowledge_.size(); p++) {
            for (int i = 0; i < hand_knowledge_[p].size(); i++) {
                hand_knowledge_[p][i].update(false);
            }
        }
    } while (update_located_count());
    update_eyesight_count(s);
}


/*
Update the knowledge for every player before a card is played,
taking into account the play's outcome.
*/
void smartbot::observe_before_play(State s, move m) {
    Card c = s.get_hands()[m.get_from()][m.get_card_index()];
    bool success = is_playable(s, c);
    no_warning_given(m.get_from(), s);

    for (int i = 0; i < hand_knowledge_.size(); i++) {
        for (int j = 0; j < hand_knowledge_[i].size(); j++) {
            hand_knowledge_[i][j].befuddle_by_play(success);
        }
    }

    played_count_[c.color()][c.rank()]++;
    shift_knowledge(m.get_from(), m.get_card_index(), s.get_deck().size() != 0);
}


/*
Update the knowledge for every player before a card is discarded.
*/
void smartbot::observe_before_discard(State s, move m) {
    Card c = s.get_hands()[m.get_from()][m.get_card_index()];
    no_warning_given(m.get_from(), s);
    if (hand_knowledge_[m.get_from()][m.get_card_index()].known() && hand_knowledge_[m.get_from()][m.get_card_index()].playable(s) == YES) {
        // Discarding a known playable card means a discard finesse. Someone else has the same card as their newest card. Check who.
        bool seen = false;
        for (int p = 0; p < hand_knowledge_.size(); p++) {
            if (p == m.get_from() || p == id_) {
                continue;
            }
            if (s.get_hands()[p].back() == c) {
                seen = true;
                hand_knowledge_[p].back().set_must_be(c);
            }
        }
        if (!seen) { // so I must have that card
            hand_knowledge_[id_].back().set_must_be(c);
        }
    }

    // Update knowledge for all players
    for (int i = 0; i < hand_knowledge_.size(); i++) {
        for (int j = 0; j < hand_knowledge_[i].size(); j++) {
            hand_knowledge_[i][j].befuddle_by_discard();
        }
    }

    // Track the discarded card and shift knowledge
    played_count_[c.color()][c.rank()]++;
    shift_knowledge(m.get_from(), m.get_card_index(), s.get_deck().size() != 0);
}


bool search(std::vector<int> indices, int index) {
    for (int i : indices) {
        if (index == i) return true;
    }
    return false;
}


/*
Process a color hint given to a player and updates the bot's
hand knowledge accordingly.
*/
void smartbot::observe_color_hint(State s, move m) {
    bool identified = false;  // tracks if the hint identifies an unambiguously playable card
    int inferred = -1;  // tracks an ambiguous card that might be played

    for (int i = s.get_hands()[m.get_to()].size() - 1; i >= 0; i--) {
        bool was_maybe_playable = (hand_knowledge_[m.get_to()][i].playable(s) == MAYBE);

        if (search(m.get_card_indices(), i)) {
            hand_knowledge_[m.get_to()][i].set_must_be(m.get_color());
            if (was_maybe_playable) {
                if (hand_knowledge_[m.get_to()][i].playable(s) == YES) { // if it is now playable, this was what your partner was trying to hint at
                    identified = true;
                } else if (hand_knowledge_[m.get_to()][i].playable(s) == MAYBE) { // if you still don't know, maybe you can infer it is playble
                    if (inferred == -1) { // only if another card has not already been inferred
                        inferred = i;
                    }
                }
            }
        } else {
            hand_knowledge_[m.get_to()][i].set_cannot_be(m.get_color());
            if (was_maybe_playable && hand_knowledge_[m.get_to()][i].playable(s) == YES) {
                identified = true;
            }
        }
    }

    if (!identified && inferred != -1) { // so this inferred card must be playable (since no other card was made playable by this hint)
        hand_knowledge_[m.get_to()][inferred].set_is_playable(true, s);
    }
    if (m.get_to() != ((m.get_from() + 1) % hand_knowledge_.size())) { // warning is only given to most immediately next partner
        no_warning_given(m.get_from(), s);
    }
}


/*
Process a rank hint given to a player and updates the bot's hand knowledge accordingly.
Also implements additional strategic logic to detect the possible reason why the hint was given.
*/
void smartbot::observe_rank_hint(State s, move m) {
    int discard_ind = next_discard_index(s, (m.get_from() + 1) % hand_knowledge_.size());
    bool hint_stone_reclaim =
        (s.get_num_hints() == 8) &&
        (m.get_from() == (m.get_to() + 1) % hand_knowledge_.size()) &&
        search(m.get_card_indices(), 0); // were they trying to just re-claim a hint stone? (check the play function)
    
    bool is_warning =
        !hint_stone_reclaim &&
        (m.get_to() == (m.get_from() + 1) % hand_knowledge_.size()) &&
        search(m.get_card_indices(), discard_ind) &&
        hand_knowledge_[m.get_to()][discard_ind].could_be_valuable(m.get_rank(), s); // is it a warning to their most immediately next partner?

    if (is_warning) {
        hand_knowledge_[m.get_to()][discard_ind].set_is_valuable(true, s);
    }

    // same as observe_color_hint
    bool identified = false;
    int inferred = -1;
    for (int i = s.get_hands()[m.get_to()].size() - 1; i >= 0; i--) {
        bool was_maybe_playable = (hand_knowledge_[m.get_to()][i].playable(s) == MAYBE);
        if (search(m.get_card_indices(), i)) {
            hand_knowledge_[m.get_to()][i].set_must_be(m.get_rank());
            if (was_maybe_playable) {
                if (hand_knowledge_[m.get_to()][i].playable(s) == YES) {
                    identified = true;
                } else if (hand_knowledge_[m.get_to()][i].playable(s) == MAYBE) {
                    if (inferred == -1) {
                        inferred = i;
                    }
                }
            }
        } else {
            hand_knowledge_[m.get_to()][i].set_cannot_be(m.get_rank());
            if (was_maybe_playable && hand_knowledge_[m.get_to()][i].playable(s) == YES) {
                identified = true;
            }
        }
    }

    if (!is_warning && !hint_stone_reclaim && !identified && inferred != -1) {
        hand_knowledge_[m.get_to()][inferred].set_is_playable(true, s);
    }
    if (m.get_to() != (m.get_from() + 1) % hand_knowledge_.size()) {
        no_warning_given(m.get_from(), s);
    }
}


move smartbot::discard_finesse(State s) {
    if (s.get_num_hints() == 8) { // can't discard
        move m = move(INVALID_MOVE);
        return m;
    }

    // what are my playable cards
    std::vector<Card> playable_cards;
    std::vector<int> playable_indices;
    for (int i = 0; i < hand_knowledge_[id_].size(); i++) {
        if (hand_knowledge_[id_][i].known() && hand_knowledge_[id_][i].valuable(s) == NO && hand_knowledge_[id_][i].playable(s) == YES) {
            playable_cards.push_back(hand_knowledge_[id_][i].known_card());
            playable_indices.push_back(i);
        }
    }

    if (playable_cards.empty()) { // can't do finesse if no playable cards
        move m = move(INVALID_MOVE);
        return m;
    }

    std::vector<Card> partner_newest; // the newest cards of each of my partners
    for (int i = 1; i < hand_knowledge_.size(); i++) {
        partner_newest.push_back(s.get_hands()[(id_ + i) % hand_knowledge_.size()].back());
    }

    // Check for a finesse opportunity:
    // For each playable card in my hand, count how many partners have the same card as their newest
    for (int i = 0; i < playable_cards.size(); i++) {
        int count = 0;
        for (int j = 0; j < partner_newest.size(); j++) {
            if (partner_newest[j] == playable_cards[i]) {
                count++;
            }
        }
        if (count == 1) { // exactly one partner's newest card is the exact same as a playable card I hold.
                          // They know that I know this card is playable, so if I discard it, they will realize they hold the exact same card.
            move m = move(DISCARD, id_, playable_indices[i]);
            return m;
        }
    }
    move m = move(INVALID_MOVE);
    return m;
}


/*
Play the lowest ranked card of any color in the current player hand.
*/
move smartbot::play_lowest_playable(State s) {
    int best_i = -1;
    double best_f = 0.0;
    for (int i = 0; i < s.get_hands()[id_].size(); i++) {
        if (hand_knowledge_[id_][i].playable(s) == NO) {
            continue;
        }
        smart_cardknowledge eye_knowl = hand_knowledge_[id_][i];
        eye_knowl.update(true);
        if (eye_knowl.playable(s) != YES) {
            continue;
        }
        double f = 6 - eye_knowl.rank();
        if (hand_knowledge_[id_][i].playable(s) != YES) {
            f += 100;
        }
        if (f > best_f) {
            best_f = f;
            best_i = i;
        }
    }
    if (best_i != -1) { // found a lowest playable card
        move m = move(PLAY, id_, best_i);
        return m;
    } else {
        move m = move(INVALID_MOVE);
        return m;
    }
}


move smartbot::discard_worthless(State s) {
    int best_i = -1;
    double best_f = 0.0;
    for (int i = 0; i < s.get_hands()[id_].size(); i++) {
        if (hand_knowledge_[id_][i].worthless(s) == NO) {
            continue;
        }
        if (hand_knowledge_[id_][i].worthless(s) == MAYBE) {
            smart_cardknowledge eye_knowl = hand_knowledge_[id_][i];
            eye_knowl.update(true);
            if (eye_knowl.worthless(s) != YES) {
                continue;
            }
        }
        double f = 2.0 - hand_knowledge_[id_][i].prob_worthless(s);
        if (f > best_f) {
            best_f = f;
            best_i = i;
        }
    }
    if (best_i != -1) { // found a card with highest prob_worthless
        move m = move(DISCARD, id_, best_i);
        return m;
    } else {
        move m = move(INVALID_MOVE);
        return m;        
    }
}


/*
If the size of the deck is small compared to the remaining number of lives,
we can afford to take a risk.
This function plays the card with the highest probability of being playable.
*/
move smartbot::play_mystery(State s) {
    int table[4] = { -99, 1, 1, 3 }; // similar logic to holmes::play_mystery
    if (s.get_deck().size() <= table[s.get_num_lives()]) {
        double best_f = 0;
        int best_i = -1;
        for (int i = hand_knowledge_[id_].size() - 1; i >= 0; i--) {
            smart_cardknowledge eye_knowl = hand_knowledge_[id_][i];
            eye_knowl.update(true);
            if  (eye_knowl.playable(s) == MAYBE) {
                double f = eye_knowl.prob_playable(s);
                if (f > best_f) {
                    best_f = f;
                    best_i = i;
                }
            }
        }
        if (best_i != -1) { // play the card with highest prob_playable
            move m = move(PLAY, id_, best_i);
            return m;
        }
    }
    move m = move(INVALID_MOVE);
    return m;
}


/*
Just discard the oldest card in the hand.
The oldest card is found at the leftmost position.
*/
move smartbot::discard_old(State s) {
    int best_i = next_discard_index(s, id_);
    if (best_i != -1) {
        move m = move(DISCARD, id_, best_i);
        return m;
    }
    move m = move(INVALID_MOVE);
    return m;
}


/*************************** START OF HELPER FUNCTIONS FOR SMARTBOT ***************************/

/*
Calculates how much information we gain by going from a hand to another.
*/
int reduction_in_entropy(std::vector<smart_cardknowledge> old_knowl, std::vector<smart_cardknowledge> new_knowl) {
    int res = 0;
    for (int i = 0; i < old_knowl.size(); i++) {
        res += old_knowl[i].possibilities() - new_knowl[i].possibilities();
    }
    return res;
}


std::vector<int> search_indices(std::vector<Card> hand, Color col) {
    std::vector<int> indices = {};
    for (int i = 0; i < hand.size(); i++) {
        if (hand[i].color() == col) {
            indices.push_back(i);
        }
    }
    return indices;
}


std::vector<int> search_indices(std::vector<Card> hand, Rank rank) {
    std::vector<int> indices = {};
    for (int i = 0; i < hand.size(); i++) {
        if (hand[i].rank() == rank) {
            indices.push_back(i);
        }
    }
    return indices;
}


/*************************** END OF HELPER FUNCTIONS FOR SMARTBOT ***************************/

/*
Determines the best hint to provide to a partner,
where the best hint is defined as the hint that maximises the information gained
from the hint (as calculated by reduction_in_entropy).
*/
template<class F>
std::tuple<move, int> smartbot::best_hint_for_partner_given_constraint(State s, int partner_index, F&& is_okay) {
    std::vector<Card> partner_hand = s.get_hands()[partner_index];

    bool cols[6] = {}; // whether my partner has this color
    bool ranks[6] = {}; // ^
    for (Card &card : partner_hand) {
        cols[card.color()] = true;
        ranks[card.rank()] = true;
    }

    int best_f = 0;
    std::vector<smart_cardknowledge> old_knowl = hand_knowledge_[partner_index];
    move best = move(INVALID_MOVE);

    // Try to give a color hint
    for (int k = 1; k < 6; k++) {
        if (!cols[k]) {
            continue;
        }

        std::vector<int> indices = search_indices(partner_hand, Color(k));
        move m = move(COL_HINT, partner_index, id_, indices, Color(k));
        std::vector<smart_cardknowledge> new_knowl = old_knowl;
        for (int i = 0; i < partner_hand.size(); i++) {
            if (partner_hand[i].color() == Color(k)){
                new_knowl[i].set_must_be(Color(k));
            } else {
                new_knowl[i].set_cannot_be(Color(k));
            }
        }

        if (is_okay(m, old_knowl, new_knowl)) { // if this move won't cause any misinformation (check best_hint_for_partner for what is_okay is)
            int f = reduction_in_entropy(old_knowl, new_knowl);
            if (f > best_f) { // chose hint with highest entropy reduction
                best_f = f;
                best = m;
            }
        }
    }

    // Try to give a rank hint
    for (int r = 1; r < 6; r++) { // Same as ^
        if (!ranks[r]) {
            continue;
        }
        std::vector<int> indices = search_indices(partner_hand, Rank(r));
        move m = move(RANK_HINT, partner_index, id_, indices, Rank(r));
        std::vector<smart_cardknowledge> new_knowl = old_knowl;
        for (int i = 0; i < partner_hand.size(); i++) {
            if (partner_hand[i].rank() == Rank(r)){
                new_knowl[i].set_must_be(Rank(r));
            } else {
                new_knowl[i].set_cannot_be(Rank(r));
            }
        }
        if (is_okay(m, old_knowl, new_knowl)) {
            int f = reduction_in_entropy(old_knowl, new_knowl);
            if (f > best_f) {
                best_f = f;
                best = m;
            }
        }
    }

    return std::make_tuple(best, best_f);
}


std::tuple<move, int> smartbot::best_hint_for_partner(State s, int partner_index) {
    std::vector<Card> partner_hand = s.get_hands()[partner_index];

    bool is_actually_playable[partner_hand.size()];
    for (int i = 0; i < partner_hand.size(); i++) {
        is_actually_playable[i] = s.get_piles()[partner_hand[i].color()] + 1 == partner_hand[i].rank();
    }

    Rank avoid_rank = invalid_rank; // check holmes for explanation
    if (partner_index == (id_ + 1) % hand_knowledge_.size()) {
        int discard_ind = next_discard_index(s, partner_index);
        if (discard_ind != -1) {
            avoid_rank = partner_hand[discard_ind].rank();
            if (!hand_knowledge_[partner_index][discard_ind].could_be_valuable(avoid_rank, s)) {
                avoid_rank = invalid_rank;
            }
        }
    }


    return best_hint_for_partner_given_constraint(s, partner_index, [&](move m, std::vector<smart_cardknowledge> old_knowl, std::vector<smart_cardknowledge> new_knowl) {
        if (m.get_rank() != invalid_rank && m.get_rank() == avoid_rank) {
            return false; // will be miscontrued as a warning
        }

        bool reveals_playable = false;
        trivalue is_misleading = MAYBE;
        for (int c = partner_hand.size() - 1; c >= 0; c--) {
            if (old_knowl[c].playable(s) != MAYBE) { // they already know the playability
                continue;
            }
            if (new_knowl[c].playable(s) == YES) { // hint makes it playable
                reveals_playable = true;
            } else if (new_knowl[c].playable(s) == MAYBE && search(m.get_card_indices(), c)) {
                if (is_misleading == MAYBE) {
                    is_misleading = (is_actually_playable[c] ? NO : YES); // hint is misleading if a hinted card isn't playable
                }
            }
        }
        if (reveals_playable || is_misleading == NO) { // if the hint isn't misleading or it certainly reveals a playable card, it's okay
            return true;
        }
        return false; // otherwise, partner may be led to infer that an unplayable card is playable
    });
}


/*
Warn another player before they discard a valuable card
*/
move smartbot::give_valuable_warning(State s) { // check holmes for explanation
    if (s.get_num_hints() == 0) {
        move m = move(INVALID_MOVE);
        return m;
    }
    int player_to_warn = (id_ + 1) % hand_knowledge_.size();
    int disc_ind = next_discard_index(s, player_to_warn);
    if (disc_ind == -1) {
        move m = move(INVALID_MOVE);
        return m;
    }
    Card c = s.get_hands()[player_to_warn][disc_ind];
    if (!is_valuable(s, c)) {
        move m = move(INVALID_MOVE);
        return m;        
    }

    std::tuple<move, int> best_hint_tuple = best_hint_for_partner(s, player_to_warn);
    if (std::get<1>(best_hint_tuple) > 0) {
        return std::get<0>(best_hint_tuple);
    }
    std::vector<int> indices = search_indices(s.get_hands()[player_to_warn], c.rank());
    move m = move(RANK_HINT, player_to_warn, id_, indices, c.rank());
    return m;
}


/*
Find the best hint to give for any partner.
Here the best hint is defined as color or rank hint to any partner
that provides the largest amount of information.
*/
move smartbot::give_helpful_hint(State s) {
    if (s.get_num_hints() == 0) {
        move m = move(INVALID_MOVE);
        return m;
    }

    move best = move(INVALID_MOVE);
    int best_f = 0;
    for (int i = 1; i < hand_knowledge_.size(); i++) { // find best hint across partners
        int partner = (id_ + i) % hand_knowledge_.size();
        std::tuple<move, int> best_hint_tuple = best_hint_for_partner(s, partner);
        if (std::get<1>(best_hint_tuple) > best_f) {
            best = std::get<0>(best_hint_tuple);
            best_f = std::get<1>(best_hint_tuple);
        }
    }
    if (best_f == 0) {
        move m = move(INVALID_MOVE);
        return m;
    } else {
        return best;
    }
}


void smartbot::observe(State s, move m) {
    if (m.get_type() == PLAY) {
        observe_before_play(s, m);
    } else if (m.get_type() == DISCARD) {
        observe_before_discard(s, m);
    } else if (m.get_type() == COL_HINT) {
        observe_color_hint(s, m);
    } else if (m.get_type() == RANK_HINT) {
        observe_rank_hint(s, m);
    }
}


/*
Determine the best possible move given the current game state.
This function implements the decision making logic using all the previously defined methods.
*/
move smartbot::play(State s) {
    if (s.get_deck().size() == 0) {
        move m = play_lowest_playable(s);
        if (m.get_type() != INVALID_MOVE) {
            return m;
        }
        m = play_mystery(s);
        if (m.get_type() != INVALID_MOVE) {
            return m;
        }
    }
    move m = give_valuable_warning(s);
    if (m.get_type() != INVALID_MOVE) {
        return m;
    }
    m = discard_finesse(s);
    if (m.get_type() != INVALID_MOVE) {
        return m;
    }
    m = play_lowest_playable(s);
    if (m.get_type() != INVALID_MOVE) {
        return m;
    }
    m = give_helpful_hint(s);
    if (m.get_type() != INVALID_MOVE) {
        return m;
    }
    m = play_mystery(s);
    if (m.get_type() != INVALID_MOVE) {
        return m;
    }

    if (s.get_num_hints() == 8) { // just hint oldest card to most immediate partner
        int right_partner = (id_ + hand_knowledge_.size() - 1) % hand_knowledge_.size();
        std::vector<int> indices = search_indices(s.get_hands()[right_partner], s.get_hands()[right_partner][0].rank());
        m = move(RANK_HINT, right_partner, id_, indices, s.get_hands()[right_partner][0].rank());
        return m;
    } else { // will need to discard
        m = discard_worthless(s);
        if (m.get_type() != INVALID_MOVE) {
            return m;
        }
        m = discard_old(s);
        if (m.get_type() != INVALID_MOVE) {
            return m;
        }

        // just discard your highest rank card
        int best = 0;
        for (int i = 0; i < s.get_hands()[id_].size(); i++) {
            if (hand_knowledge_[id_][i].rank() > hand_knowledge_[id_][best].rank()) {
                best = i;
            }
        }
        m = move(DISCARD, id_, best);
        return m;
    }
}

move smartbot::play_prechosen(State s, move chosen_m) {
    return chosen_m;
}

int smartbot::get_id() {
    return id_;
}


int smartbot::get_n_cards() {
    return num_cards_;
}

std::vector<std::vector<SmartBotInternal::smart_cardknowledge>> smartbot::get_hk(){
    return hand_knowledge_;
}
