//#include <cstring>
//#include <algorithm>
//#include <cassert>
//#include "../include/simplebot.h"
//
//int col_to_num(Color col) {
//    if (col == red) return 0;
//    if (col == green) return 1;
//    if (col == blue) return 2;
//    if (col == yellow) return 3;
//    if (col == white) return 4;
//    if (col == invalid_color) return -1;
//    return -1;
//}
//
//
//int rank_to_num(Rank r) {
//    int num = -1;
//    switch(r) {
//        case one: num = 0; break;
//        case two: num = 1; break;
//        case three: num = 2; break;
//        case four: num = 3; break;
//        case five: num = 4; break;
//        case invalid_rank: break;
//    }
//    return num;
//}
//
//
//cardknowledge::cardknowledge()
//{
//    color_ = invalid_color;
//    rank_ = invalid_rank;
//    is_playable = false;
//}
//
//bool cardknowledge::must_be(Color color) { return (this->color_ == color); }
//bool cardknowledge::must_be(Rank rank) { return (this->rank_ == rank); }
//bool cardknowledge::cannot_be(Card card)  { return cantBe_[card.color()][card.rank()]; }
//bool cardknowledge::cannot_be(Color color)
//{
//    if (color_ != invalid_color) return (color_ != color);
//    for (int v = 1; v < 6; v++) {
//        if (!cantBe_[color][v]) return false;
//    }
//    return true;
//}
//
//bool cardknowledge::cannot_be(Rank rank)
//{
//    if (this->rank_ != invalid_rank) return (this->rank_ != rank);
//    for (int k = 1; k < 6; k++) {
//        if (!cantBe_[k][rank]) return false;
//    }
//    return true;
//}
//
//int cardknowledge::color() { return color_; }
//int cardknowledge::rank() { return rank_; }
//
//void cardknowledge::set_must_be(Color color)
//{
//    for (int k = 1; k < 6; k++) {
//        if (k != color) set_cannot_be(Color(k));
//    }
//    color_ = color;
//}
//
//void cardknowledge::set_must_be(Rank rank)
//{
//    for (int v = 1; v < 6; v++) {
//        if (v != rank) set_cannot_be(Rank(v));
//    }
//    rank_ = rank;
//}
//
//void cardknowledge::set_cannot_be(Color color)
//{
//    for (int v = 1; v < 6; v++) {
//        cantBe_[color][v] = true;
//    }
//}
//
//void cardknowledge::set_cannot_be(Rank rank)
//{
//    for (int k = 0; k < 6; k++) {
//        cantBe_[k][rank] = true;
//    }
//}
//
//simplebot::simplebot(int n_cards, int id, int n_players)
//{
//    num_cards_ = n_cards;
//    id_ = id;
//    hand_knowledge_.resize(n_players);
//    for (int i = 0; i < n_players; i++) {
//        hand_knowledge_[i].resize(n_cards);
//    }
//}
//
//void simplebot::invalidateKnol(int p_index, int c_index)
//{
//    /* The other cards are shifted down and a new one drawn at the end. */
//    std::vector<cardknowledge> &vec = hand_knowledge_[p_index];
//    for (int i = c_index; i+1 < vec.size(); ++i) {
//        vec[i] = vec[i+1];
//    }
//    vec.back() = cardknowledge();
//}
//
//
//void simplebot::wipeOutPlayables(Card c) {
//    for (int p = 0; p < hand_knowledge_.size(); p++) {
//        for (int i = 0; i < hand_knowledge_[p].size(); i++) {
//            if (!(hand_knowledge_[p][i].is_playable)) continue;
//            if (hand_knowledge_[p][i].cannot_be(c)) continue;
//            hand_knowledge_[p][i].is_playable = false; // it might not be playable anymore
//        }
//    }
//}
//
//void cardknowledge::update(State s, const simplebot &bot) {
//    int color = color_;
//    int rank = rank_;
//    repeat_loop:
//    if (color == invalid_color) {
//        for (int k = 1; k < 6; k++) {
//            if (cannot_be(Color(k))) continue;
//            else if (color == invalid_color) color = k;
//            else {color = invalid_color; break;}
//        }
//        if (color != invalid_color) set_must_be(Color(color));
//    }
//    if (rank == invalid_rank) {
//        for (int r = 1; r < 6; r++) {
//            if (cannot_be(Rank(r))) continue;
//            else if (rank == invalid_rank) rank = r;
//            else {rank = invalid_rank; break;}
//        }
//        if (rank != invalid_rank) set_must_be(Rank(rank));
//    }
////    if (rank == invalid_rank || color == invalid_color) {
////        bool restart = false;
////        for (int k = 1; k < 6; k++) {
////            for (int r = 1; r < 6; r++) {
////                if (this->cantBe_[k][r]) continue;
////                int total = (r == 1 ? 3 : (r == 5 ? 1 : 2));
////                int played = bot.played_count_[k][r];
////                int held = bot.located_count_[k][r];
////                if ((played + held >= total) ||(is_playable && s.get_piles()[k] + 1 != r))
////                {
////                    this->cantBe_[k][r] = true;
////                    restart = true;
////                }
////            }
////        }
////        if (restart) goto repeat_loop;
////    }
//
//
//    if (!is_playable) {
//        for (int k = 1; k < 6; k++) {
//            for (int r = 1; r < 6; r++) {
//                if (this->cantBe_[k][r]) continue;
//                if (s.get_piles()[k] + 1 != r) {
//                    goto maybeUnplayable;
//                }
//            }
//        }
//        is_playable = true;
//        maybeUnplayable:;
//    }
//}
////bool simplebot::update_located_count() {
////    int new_count[6][6] = {};
////    for (int p = 0; p < hand_knowledge_.size(); p++) {
////        for (int i = 0; i < hand_knowledge_[p].size(); i++) {
////            if (hand_knowledge_[p][i].color() != invalid_color && hand_knowledge_[p][i].rank() != invalid_rank) {
////                new_count[hand_knowledge_[p][i].color()][hand_knowledge_[p][i].rank()] += 1;
////            }
////        }
////    }
////    return false;
////}
//
//void simplebot::observe_before_move(State s) {
////    my_hand_size_ = s.get_hands()[id_].size();
////    for (int p = 0; p < hand_knowledge_.size(); p++) {
////        hand_knowledge_[p].resize(s.get_hands()[p].size());
////    }
////    lowest_playable_rank_ = 6;
////    for (int k = 1; k < 6; k++) {
////        lowest_playable_rank_ = std::min(lowest_playable_rank_, s.get_piles()[k] + 1);
////    }
//}
//
//void simplebot::observe_before_discard(State s, move m) {
////    Card c = s.get_hands()[m.get_from()][m.get_card_index()]; // can observe since we're discarding anyway, so the card is now public
////    played_count_[c.color()][c.rank()] += 1;
//    this->invalidateKnol(m.get_from(), m.get_card_index());
//}
//
//void simplebot::observe_before_play(State s, move m) {
//    Card c = s.get_hands()[m.get_from()][m.get_card_index()]; // can observe since we're playing anyway, so the card is now public
////    played_count_[c.color()][c.rank()] += 1;
//    this->invalidateKnol(m.get_from(), m.get_card_index());
//    this->wipeOutPlayables(c);
//}
//
//void simplebot::observe_color_hint(State s, move m) {
//    int next_rank = s.get_piles()[m.get_color()] + 1;
//    next_rank = std::min(next_rank, 5);
//    for (int i = 0; i < s.get_hands()[m.get_to()].size(); i++) {
//        bool found = false;
//        for (int j : m.get_card_indices()) {
//            if (j == i) found = true;
//        }
//        if (found) {
//            hand_knowledge_[m.get_to()][i].set_must_be(m.get_color());
//            if (hand_knowledge_[m.get_to()][i].rank() == invalid_rank) {
//                hand_knowledge_[m.get_to()][i].set_must_be(Rank(next_rank));
//                hand_knowledge_[m.get_to()][i].is_playable = true;
//            }
//        } else {
//            hand_knowledge_[m.get_to()][i].set_cannot_be(m.get_color());
//        }
//    }
//}
//
//void simplebot::observe_rank_hint(State s, move m) {
//    bool zero_found = false;
//    for (int j : m.get_card_indices()) {
//        if (j == 0) zero_found = true;
//    }
//    if (s.get_num_hints() == 8 && m.get_from() == (m.get_to() + 1) % s.get_hands().size() && zero_found) return;
//    std::cout << m.get_card_indices().size() << std::endl;
//    for (int i = 0; i < s.get_hands()[m.get_to()].size(); i++) {
//        bool found = false;
//        std::cout << "searching" << std::endl;
//        for (int j : m.get_card_indices()) {
//            std::cout << j << i << std::endl;
//            if (j == i) found = true;
//        }
//        if (found) {
//            hand_knowledge_[m.get_to()][i].set_must_be(m.get_rank());
//            if (hand_knowledge_[m.get_to()][i].color() == invalid_color) {
//                std::cout << "HERE" << std::endl;
//                std::cout << m.get_to() << std::endl;
//                std::cout << m.get_rank() << std::endl;
//                std::cout << i << std::endl;
//                hand_knowledge_[m.get_to()][i].is_playable = true;
//            }
//        } else {
//            hand_knowledge_[m.get_to()][i].set_cannot_be(m.get_rank());
//        }
//    }
//}
//
//void simplebot::observe(State s, move m) {
//    observe_before_move(s);
//    if (m.get_type() == PLAY) {
//        observe_before_play(s, m);
//    } else if (m.get_type() == DISCARD) {
//        observe_before_discard(s, m);
//    } else if (m.get_type() == COL_HINT) {
//        observe_color_hint(s, m);
//    }
//    else if (m.get_type() == RANK_HINT) {
//        observe_rank_hint(s, m);
//    }
//}
//
//
//
//move simplebot::play_lowest_playable(State s) {
//    int best_i = -1;
//    int lowest_r = 6;
//    for (int i = 0; i < s.get_hands()[id_].size(); i++) {
//        if (hand_knowledge_[id_][i].is_playable && hand_knowledge_[id_][i].rank() < lowest_r) {
//            best_i = i;
//            lowest_r = hand_knowledge_[id_][i].rank();
//        }
//    }
//    if (best_i != -1) {
//        std::cout << rank_to_num(s.get_hands()[id_][best_i].rank()) << std::endl;
//        move m = move(PLAY, -1, id_, best_i, {}, invalid_color, invalid_rank);
//        return m;
//    }
//    move m = move(INVALID_MOVE, -1, -1, -1, {}, invalid_color, invalid_rank);
//    return m;
//}
//
//
//std::tuple<move, int> simplebot::best_hint_for_partner(State s, int partner_index) {
//    std::vector<Card> partner_hand = s.get_hands()[partner_index];
//    bool is_actually_playable[partner_hand.size()];
//    for (int i = 0; i < partner_hand.size(); i++) {
//        is_actually_playable[i] = s.get_piles()[partner_hand[i].color()] + 1 == partner_hand[i].rank();
//    }
//    move m = move(INVALID_MOVE, -1, -1, -1, {}, invalid_color, invalid_rank);
//    int highest_info = 0;
//    for (int k = 1; k < 6; k++) {
//        int info = 0;
//        bool misinformative = false;
//        std::vector<int> card_indices = {};
//        for (int i = 0; i < partner_hand.size(); i++) {
//            if (partner_hand[i].color() != k) continue;
//            if (is_actually_playable[i] && !(hand_knowledge_[partner_index][i].is_playable)) {
//                info++;
//            }
////            else if (!is_actually_playable[i] && (hand_knowledge_[partner_index][i].rank() == invalid_rank)) {
////                misinformative = true;
////                break;
////            }
//            card_indices.push_back(i);
//        }
////        if (misinformative) continue;
////        if (info > highest_info) {
////            highest_info = info;
////            m = move(COL_HINT, partner_index, id_, -1, card_indices, Color(k), invalid_rank);
////        }
//    }
//
//    //int discard_index = next_discard_index(s, partner_index);
//    Rank avoid_rank = invalid_rank;
////    if (discard_index != -1) {
////        avoid_rank = partner_hand[discard_index].rank();
////        if(!could_be_valuable(s, hand_knowledge_[partner_index][discard_index], avoid_rank)) avoid_rank = invalid_rank;
////    }
//    for (int r = 1; r < 6; r++) {
//        if (r == avoid_rank) continue;
//        int info = 0;
//        bool misinformative = false;
//        std::vector<int> card_indices = {};
//        for (int i = 0; i < partner_hand.size(); i++) {
//            if (partner_hand[i].rank() != r) continue;
//            if (is_actually_playable[i] && !(hand_knowledge_[partner_index][i].is_playable)) {
//                info++;
//            }
////            else if (!is_actually_playable[i] && (hand_knowledge_[partner_index][i].color() == invalid_color)) {
////                misinformative = true;
////                std::cout << "misinf" << std::endl;
////                break;
////            }
//            card_indices.push_back(i);
//        }
//        if (misinformative) continue;
//        if (info > highest_info) {
//            highest_info = info;
//            m = move(RANK_HINT, partner_index, id_, -1, card_indices, invalid_color, Rank(r));
//        }
//    }
//    std::cout << highest_info << std::endl;
//    return std::make_tuple(m, highest_info);
//}
//
//
//
//move simplebot::give_helpful_hint(State s) {
//    if (s.get_num_hints() == 0) {
//        move m = move(INVALID_MOVE, -1, -1, -1, {}, invalid_color, invalid_rank);
//        return m;
//    }
//    move best_hint = move(INVALID_MOVE, -1, -1, -1, {}, invalid_color, invalid_rank);;
//    int highest_info = 0;
//    for (int i = 1; i < hand_knowledge_.size(); i++) {
//        int partner = (id_ + i) % hand_knowledge_.size();
//        std::tuple<move, int> best_hint_tuple = best_hint_for_partner(s, partner);
//        if (std::get<0>(best_hint_tuple).get_type() == INVALID_MOVE) continue;
//        if (std::get<1>(best_hint_tuple) > highest_info) {
//            highest_info = std::get<1>(best_hint_tuple);
//            best_hint = std::get<0>(best_hint_tuple);
//        }
//    }
//    if (best_hint.get_type() == INVALID_MOVE) {
//        move m = move(INVALID_MOVE, -1, -1, -1, {}, invalid_color, invalid_rank);
//        return m;
//    }
//    return best_hint;
//}
//
//move simplebot::discard_old(State s) {
//    for (int i = 0; i < my_hand_size_; i++) {
//        if (hand_knowledge_[id_][i].is_playable) continue;
//        move m = move(DISCARD, -1, id_, i, {}, invalid_color, invalid_rank);
//        return m;
//    }
//    move m = move(INVALID_MOVE, -1, -1, -1, {}, invalid_color, invalid_rank);
//    return m;
//}
//
////move simplebot::play_mystery(State s) {
////    int table[4] = { -99, 1, 1, 1 };
////    if (s.get_deck().size() <= table[s.get_num_lives()]) {
////        for (int i = my_hand_size_ - 1; i >= 0; i--) {
////            if ((hand_knowledge_[id_][i].color() != invalid_color && hand_knowledge_[id_][i].rank() != invalid_rank)) continue;
////            move m = move(PLAY, -1, id_, i, {}, invalid_color, invalid_rank);
////            return m;
////        }
////    }
////    move m = move(INVALID_MOVE, -1, -1, -1, {}, invalid_color, invalid_rank);
////    return m;
////}
//
//move simplebot::play(State s)
//{
//    /* If I have a playable card, play it.
//     * Otherwise, if someone else has an unknown-playable card, hint it.
//     * Otherwise, just discard my oldest (index-0) card. */
//
//    move m = play_lowest_playable(s);
//    if (m.get_type() != INVALID_MOVE) {
//        std::cout << "lowestplay" << std::endl;
//        std::cout << col_to_num(s.get_hands()[id_][m.get_card_index()].color()) << rank_to_num(s.get_hands()[id_][m.get_card_index()].rank()) << std::endl;
//        std::cout << m.get_card_index() << std::endl;
//        return m;
//    }
//    m = give_helpful_hint(s);
//    if (m.get_type() != INVALID_MOVE) {
//        std::cout << "helpful" << std::endl;
//        return m;
//    }
////    m = play_mystery(s);
////    if (m.get_type() != INVALID_MOVE) {
////        std::cout << "mystery" << std::endl;
////        return m;
////    }
//
//    if (s.get_num_hints() == 8) {
//        int right_partner = (id_ + hand_knowledge_.size() - 1) % hand_knowledge_.size();
//        Rank r = s.get_hands()[right_partner][0].rank();
//        std::vector<int> card_indices = {};
//        for (int i = 0; i < s.get_hands()[right_partner].size(); i++) {
//            if (s.get_hands()[right_partner][i].rank() == r) {
//                if (i == 0) std::cout << "ind con 0" << std::endl;
//                card_indices.push_back(i);
//            }
//        }
//        m = move(RANK_HINT, right_partner, id_, -1, card_indices, invalid_color, r);
//        return m;
//    } else {
//        m = discard_old(s);
//        if (m.get_type() != INVALID_MOVE) {
//            std::cout << "old" << std::endl;
//            return m;
//        }
//        int best = 0;
//        for (int i = 0; i < my_hand_size_; i++) {
//            if (hand_knowledge_[id_][i].rank() > hand_knowledge_[id_][best].rank()) best = i;
//        }
//        m = move(DISCARD, -1, id_, best, {}, invalid_color, invalid_rank);
//        return m;
//    }
//}