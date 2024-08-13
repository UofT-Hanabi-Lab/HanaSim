#include <cstring>
#include <algorithm>
#include <assert.h>

#include "include/holmesbot.h"

using namespace HolmesBotInternal;

cardknowledge::cardknowledge()
{
    color_ = invalid_color;
    rank_ = invalid_rank;
    memset(cant_be_, '\0', sizeof cant_be_);
    is_playable = false;
    is_valuable = false;
    is_worthless = false;
}

bool cardknowledge::must_be(Color color) { return (this->color_ == color); }
bool cardknowledge::must_be(Rank rank) { return (this->rank_ == rank); }
bool cardknowledge::cannot_be(Card card)  { return cant_be_[card.color()][card.rank()]; }
bool cardknowledge::cannot_be(Color color)
{
    if (color_ != invalid_color) return (color_ != color);
    for (int v = 1; v < 6; v++) {
        if (!cant_be_[color][v]) return false;
    }
    return true;
}

bool cardknowledge::cannot_be(Rank rank)
{
    if (this->rank_ != invalid_rank) return (this->rank_ != rank);
    for (int k = 1; k < 6; k++) {
        if (!cant_be_[k][rank]) return false;
    }
    return true;
}

int cardknowledge::color() { return color_; }
int cardknowledge::rank() { return rank_; }

void cardknowledge::set_must_be(Color color)
{
    for (int k = 1; k < 6; k++) {
        if (k != color) set_cannot_be(Color(k));
    }
    color_ = color;
}

void cardknowledge::set_must_be(Rank rank)
{
    for (int v = 1; v < 6; v++) {
        if (v != rank) set_cannot_be(Rank(v));
    }
    rank_ = rank;
}

void cardknowledge::set_cannot_be(Color color)
{
    for (int v = 1; v < 6; v++) {
        cant_be_[color][v] = true;
    }
}

void cardknowledge::set_cannot_be(Rank rank)
{
    for (int k = 0; k < 6; k++) {
        cant_be_[k][rank] = true;
    }
}

void cardknowledge::update(State s, const holmesbot &bot) {
    int color = color_;
    int rank = rank_;
    repeat_loop:
    if (color == invalid_color) {
        for (int k = 1; k < 6; k++) {
            if (cannot_be(Color(k))) continue;
            else if (color == invalid_color) color = k;
            else {color = invalid_color; break;}
        }
        if (color != invalid_color) set_must_be(Color(color));
    }
    if (rank == invalid_rank) {
        for (int r = 1; r < 6; r++) {
            if (cannot_be(Rank(r))) continue;
            else if (rank == invalid_rank) rank = r;
            else {rank = invalid_rank; break;}
        }
        if (rank != invalid_rank) set_must_be(Rank(rank));
    }
    if (rank == invalid_rank || color == invalid_color) {
        bool restart = false;
        for (int k = 1; k < 6; k++) {
            for (int r = 1; r < 6; r++) {
                if (this->cant_be_[k][r]) continue;
                int total = (r == 1 ? 3 : (r == 5 ? 1 : 2));
                int played = bot.played_count_[k][r];
                int held = bot.located_count_[k][r];
                if ((played + held >= total) ||
                    (is_valuable && !bot.is_valuable(s, Card(Color(k), Rank(r)))) ||
                    (is_playable && s.get_piles()[k] + 1 != r) ||
                    (is_worthless && s.get_piles()[k] < r))
                {
                    this->cant_be_[k][r] = true;
                    restart = true;
                }
            }
        }
        if (restart) goto repeat_loop;
    }
    if (is_worthless) return;

    if (!is_playable && !is_valuable) {
        for (int k = 1; k < 6; k++) {
            for (int r = 1; r < 6; r++) {
                if (cant_be_[k][r]) continue;
                if (r > s.get_piles()[k]) {
                    goto notWorthless;
                }
            }
        }
        is_worthless = true;
        return;
      notWorthless:;
    }

    if (!is_valuable) {
        for (int k = 1; k < 6; k++) {
            for (int r = 1; r < 6; r++) {
                if (this->cant_be_[k][r]) continue;
                if (!bot.is_valuable(s, Card(Color(k), Rank(r)))) {
                    goto maybeNotValuable;
                }
            }
        }
        is_valuable = true;
      maybeNotValuable:;
    }

    if (!is_playable) {
        for (int k = 1; k < 6; k++) {
            for (int r = 1; r < 6; r++) {
                if (this->cant_be_[k][r]) continue;
                if (s.get_piles()[k] + 1 != r) {
                    goto maybeUnplayable;
                }
            }
        }
        is_playable = true;
      maybeUnplayable:;
    }
}

holmesbot::holmesbot(int id, int n_players) {
    num_cards_ = (n_players <= 3) ? 5 : 4;;
    id_ = id;
    hand_knowledge_.resize(n_players);
    for (int i = 0; i < n_players; i++) {
        hand_knowledge_[i].resize(num_cards_);
    }
    std::memset(played_count_, '\0', sizeof played_count_);
}

bool holmesbot::is_valuable(State s, Card card) const {
    if (s.get_piles()[card.color()] >= card.rank()) return false;
    int total = (card.rank() == 1 ? 3 : (card.rank() == 5 ? 1 : 2));
    return (played_count_[card.color()][card.rank()] == total - 1); // Valuable if it's the last card of its kind
}

bool holmesbot::could_be_valuable(State s, cardknowledge ck, int rank) {
    for (int k = 1; k < 6; k++) {
        if (ck.cannot_be(Card(Color(k), Rank(rank)))) continue;
        if (is_valuable(s, Card(Color(k), Rank(rank)))) return true;
    }
    return false;
}

void holmesbot::shift_knowledge(int p_index, int c_index) {
    std::vector<cardknowledge> &vec = hand_knowledge_[p_index];
    for (int i = c_index; i + 1 < vec.size(); i++) {
        vec[i] = vec[i + 1];
    }
    vec.back() = cardknowledge();
}

bool holmesbot::update_located_count() {
    int new_count[6][6] = {};
    for (int p = 0; p < hand_knowledge_.size(); p++) {
        for (int i = 0; i < hand_knowledge_[p].size(); i++) {
            if (hand_knowledge_[p][i].color() != invalid_color && hand_knowledge_[p][i].rank() != invalid_rank) {
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

void holmesbot::observe_before_move(State s) {
    my_hand_size_ = s.get_hands()[id_].size();
    for (int p = 0; p < hand_knowledge_.size(); p++) {
        hand_knowledge_[p].resize(s.get_hands()[p].size());
    }
    std::memset(located_count_, '\0', sizeof located_count_);
    update_located_count();
    do {
        for (int p = 0; p < hand_knowledge_.size(); p++) {
            for (int i = 0; i < hand_knowledge_[p].size(); i++) {
                hand_knowledge_[p][i].update(s, *this);
            }
        }
    } while (update_located_count());
    lowest_playable_rank_ = 6;
    for (int k = 1; k < 6; k++) {
        lowest_playable_rank_ = std::min(lowest_playable_rank_, s.get_piles()[k] + 1);
    }
}

void holmesbot::observe_before_discard(State s, move m) {
    Card c = s.get_hands()[m.get_from()][m.get_card_index()]; // can observe since we're discarding anyway, so the card is now public
    played_count_[c.color()][c.rank()] += 1;
    shift_knowledge(m.get_from(), m.get_card_index());
}

void holmesbot::wipe_out_playables(Card c) {
    for (int p = 0; p < hand_knowledge_.size(); p++) {
        for (int i = 0; i < hand_knowledge_[p].size(); i++) {
            if(!(hand_knowledge_[p][i].is_playable)) continue;
            if (hand_knowledge_[p][i].is_valuable) continue;
            if (hand_knowledge_[p][i].cannot_be(c)) continue;
            hand_knowledge_[p][i].is_playable = false; // it might not be playable anymore
        }
    }
}

void holmesbot::observe_before_play(State s, move m) {
    Card c = s.get_hands()[m.get_from()][m.get_card_index()]; // can observe since we're playing anyway, so the card is now public
    if (s.get_piles()[c.color()] + 1 == c.rank()) {
//         This card is getting played, not discarded.
        if (!is_valuable(s, c)) {
            wipe_out_playables(c);
        }
    }
    played_count_[c.color()][c.rank()] += 1;
    shift_knowledge(m.get_from(), m.get_card_index());
}

bool search_for_ind(std::vector<int> indices, int index) {
    for (int i : indices) {
        if (index == i) return true;
    }
    return false;
}

void holmesbot::observe_color_hint(State s, move m) {
    int next_rank = s.get_piles()[m.get_color()] + 1;
    next_rank = std::min(next_rank, 5);
    for (int i = 0; i < s.get_hands()[m.get_to()].size(); i++) {
        if (search_for_ind(m.get_card_indices(), i)) {
            hand_knowledge_[m.get_to()][i].set_must_be(m.get_color());
            if (hand_knowledge_[m.get_to()][i].rank() == invalid_rank && !(hand_knowledge_[m.get_to()][i].is_worthless)) {
                hand_knowledge_[m.get_to()][i].set_must_be(Rank(next_rank));
                hand_knowledge_[m.get_to()][i].is_playable = true;
            }
        } else {
            hand_knowledge_[m.get_to()][i].set_cannot_be(m.get_color());
        }
    }
}

int holmesbot::next_discard_index(State s, int player_index) {
    for (int i = 0; i < s.get_hands()[player_index].size(); i++) {
        if (hand_knowledge_[player_index][i].is_worthless || hand_knowledge_[player_index][i].is_playable) return -1;
    }
    for (int i = 0; i < s.get_hands()[player_index].size(); i++) {
        if (!(hand_knowledge_[player_index][i].is_valuable)) return i;
    }
    return -1;
}

void holmesbot::observe_rank_hint(State s, move m) {
    int discard_ind = next_discard_index(s, m.get_to());
    bool is_warning = search_for_ind(m.get_card_indices(), discard_ind) && could_be_valuable(s, hand_knowledge_[m.get_to()][discard_ind], m.get_rank());
    if (s.get_num_hints() == 8 && m.get_from() == (m.get_to() + 1) % s.get_hands().size() && search_for_ind(m.get_card_indices(), 0)) return;
    if (is_warning) {
        hand_knowledge_[m.get_to()][discard_ind].is_valuable = true;
        if (m.get_rank() == lowest_playable_rank_) {
            hand_knowledge_[m.get_to()][discard_ind].is_playable = true;
        }
    }
    for (int i = 0; i < s.get_hands()[m.get_to()].size(); i++) {
        if (search_for_ind(m.get_card_indices(), i)) {
            hand_knowledge_[m.get_to()][i].set_must_be(m.get_rank());
            if (hand_knowledge_[m.get_to()][i].color() == invalid_color && !is_warning && !hand_knowledge_[m.get_to()][i].is_worthless) {
                hand_knowledge_[m.get_to()][i].is_playable = true;
            }
        } else {
            hand_knowledge_[m.get_to()][i].set_cannot_be(m.get_rank());
        }
    }
}

void holmesbot::observe(State s, move m) {
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

move holmesbot::play_lowest_playable(State s) {
    int best_i = -1;
    int lowest_r = 6;
    for (int i = 0; i < s.get_hands()[id_].size(); i++) {
        if (hand_knowledge_[id_][i].is_playable && hand_knowledge_[id_][i].rank() < lowest_r) {
            best_i = i;
            lowest_r = hand_knowledge_[id_][i].rank();
        }
    }
    if (best_i != -1) {
        move m = move(PLAY, id_, best_i);
        return m;
    }
    move m = move(INVALID_MOVE);
    return m;
}

move holmesbot::discard_worthless(State s) {
    for (int i = 0; i < s.get_hands()[id_].size(); i++) {
        if (hand_knowledge_[id_][i].is_worthless) {
            move m = move(DISCARD, id_, i);
            return m;
        }
    }
    move m = move(INVALID_MOVE);
    return m;
}

move holmesbot::play_mystery(State s) {
    int table[4] = { -99, 1, 1, 1 };
    if (s.get_deck().size() <= table[s.get_num_lives()]) {
        for (int i = my_hand_size_ - 1; i >= 0; i--) {
            if (hand_knowledge_[id_][i].is_worthless || (hand_knowledge_[id_][i].color() != invalid_color && hand_knowledge_[id_][i].rank() != invalid_rank)) continue;
            move m = move(PLAY, id_, i);
            return m;
        }
    }
    move m = move(INVALID_MOVE);
    return m;
}

move holmesbot::discard_old(State s) {
    for (int i = 0; i < my_hand_size_; i++) {
        if (hand_knowledge_[id_][i].is_valuable) continue;
        move m = move(DISCARD, id_, i);
        return m;
    }
    move m = move(INVALID_MOVE);
    return m;
}

std::tuple<move, int> holmesbot::best_hint_for_partner(State s, int partner_index) {
    std::vector<Card> partner_hand = s.get_hands()[partner_index];
    bool is_actually_playable[partner_hand.size()];
    for (int i = 0; i < partner_hand.size(); i++) {
        is_actually_playable[i] = s.get_piles()[partner_hand[i].color()] + 1 == partner_hand[i].rank();
    }
    move m = move(INVALID_MOVE);
    int highest_info = 0;
    for (int k = 1; k < 6; k++) {
        int info = 0;
        bool misinformative = false;
        std::vector<int> card_indices = {};
        for (int i = 0; i < partner_hand.size(); i++) {
            if (partner_hand[i].color() != k) continue;
            if (is_actually_playable[i] && !(hand_knowledge_[partner_index][i].is_playable)) {
                info++;
            } else if (!is_actually_playable[i] && (hand_knowledge_[partner_index][i].rank() == invalid_rank && !(hand_knowledge_[partner_index][i].is_worthless))) {
                misinformative = true;
                break;
            }
            card_indices.push_back(i);
        }
        if (misinformative) continue;
        if (info > highest_info) {
            highest_info = info;
            m = move(COL_HINT, partner_index, id_, card_indices, Color(k));
        }
    }

    int discard_index = next_discard_index(s, partner_index);
    Rank avoid_rank = invalid_rank;
    if (discard_index != -1) {
        avoid_rank = partner_hand[discard_index].rank();
        if(!could_be_valuable(s, hand_knowledge_[partner_index][discard_index], avoid_rank)) avoid_rank = invalid_rank;
    }
    for (int r = 1; r < 6; r++) {
        if (r == avoid_rank) continue;
        int info = 0;
        bool misinformative = false;
        std::vector<int> card_indices = {};
        for (int i = 0; i < partner_hand.size(); i++) {
            if (partner_hand[i].rank() != r) continue;
            if (is_actually_playable[i] && !(hand_knowledge_[partner_index][i].is_playable)) {
                info++;
            } else if (!is_actually_playable[i] && (hand_knowledge_[partner_index][i].color() == invalid_color && !(hand_knowledge_[partner_index][i].is_worthless))) {
                misinformative = true;
                break;
            }
            card_indices.push_back(i);
        }
        if (misinformative) continue;
        if (info > highest_info) {
            highest_info = info;
            m = move(RANK_HINT, partner_index, id_, card_indices, Rank(r));
        }
    }
    return std::make_tuple(m, highest_info);
}

move holmesbot::give_valuable_warning(State s) {
    int player_to_warn = (id_ + 1) % hand_knowledge_.size();
    int discard_index = next_discard_index(s, player_to_warn);
    if (discard_index == -1) {
        move m = move(INVALID_MOVE);
        return m;
    }
    if (!(is_valuable(s, s.get_hands()[player_to_warn][discard_index]))) {
        move m = move(INVALID_MOVE);
        return m;
    }
    if (s.get_num_hints() == 0) {
        move m = move(INVALID_MOVE);
        return m;
    }
    move best_hint = std::get<0>(best_hint_for_partner(s, player_to_warn));
    if (best_hint.get_type() != INVALID_MOVE) {
        return best_hint;
    }
    std::vector<int> card_indices = {};
    for (int i = 0; i < s.get_hands()[player_to_warn].size(); i++) {
        if (s.get_hands()[player_to_warn][i].rank() == s.get_hands()[player_to_warn][discard_index].rank()) {
            card_indices.push_back(i);
        }
    }

    move m  = move(RANK_HINT, player_to_warn, id_, card_indices, s.get_hands()[player_to_warn][discard_index].rank());
    return m;
}

move holmesbot::give_helpful_hint(State s) {
    if (s.get_num_hints() == 0) {
        move m = move(INVALID_MOVE);
        return m;
    }
    move best_hint = move(INVALID_MOVE);
    int highest_info = 0;
    for (int i = 1; i < hand_knowledge_.size(); i++) {
        int partner = (id_ + i) % hand_knowledge_.size();
        std::tuple<move, int> best_hint_tuple = best_hint_for_partner(s, partner);
        if (std::get<0>(best_hint_tuple).get_type() == INVALID_MOVE) continue;
        if (std::get<1>(best_hint_tuple) > highest_info) {
            highest_info = std::get<1>(best_hint_tuple);
            best_hint = std::get<0>(best_hint_tuple);
        }
    }
    if (best_hint.get_type() == INVALID_MOVE) {
        move m = move(INVALID_MOVE);
        return m;
    }
    return best_hint;
}

move holmesbot::play(State s) {
    move m = give_valuable_warning(s);
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

    if (s.get_num_hints() == 8) {
        int right_partner = (id_ + hand_knowledge_.size() - 1) % hand_knowledge_.size();
        Rank r = s.get_hands()[right_partner][0].rank();
        std::vector<int> card_indices = {};
        for (int i = 0; i < s.get_hands()[right_partner].size(); i++) {
            if (s.get_hands()[right_partner][i].rank() == r) {
                card_indices.push_back(i);
            }
        }
        m = move(RANK_HINT, right_partner, id_, card_indices, r);
        return m;
    } else {
        m = discard_worthless(s);
        if (m.get_type() != INVALID_MOVE) {
            return m;
        }
        m = discard_old(s);
        if (m.get_type() != INVALID_MOVE) {
            return m;
        }
        int best = 0;
        for (int i = 0; i < my_hand_size_; i++) {
            if (hand_knowledge_[id_][i].rank() > hand_knowledge_[id_][best].rank()) best = i;
        }
        m = move(DISCARD, id_, best);
        return m;
    }
}

int holmesbot::get_id() {
    return id_;
}

int holmesbot::get_n_cards() {
    return num_cards_;
}
