#include <tuple>
#include <iostream>

#include "../../include/player.h"


#pragma once

class holmesbot;

namespace HolmesBotInternal {


class cardknowledge {
    bool cant_be_[6][6];
    Color color_;
    Rank rank_;
public:
    cardknowledge();

    bool must_be(Color color);
    bool must_be(Rank rank);
    bool cannot_be(Card card);
    bool cannot_be(Color color);
    bool cannot_be(Rank rank);
    int color();
    int rank();

    void set_must_be(Color color);
    void set_must_be(Rank rank);
    void set_cannot_be(Color color);
    void set_cannot_be(Rank rank);
    void update(State s, const holmesbot &bot);

    bool is_playable;
    bool is_valuable;
    bool is_worthless;
};
}
class holmesbot final : public player {
    friend class HolmesBotInternal::cardknowledge;

    int num_cards_;
    int id_;
    int located_count_[6][6];
    int played_count_[6][6];
    std::vector<std::vector<HolmesBotInternal::cardknowledge>> hand_knowledge_; // what each player knows about their hand
    int lowest_playable_rank_;
    int my_hand_size_;

    bool is_valuable(State s, Card card) const;
    bool could_be_valuable(State s, HolmesBotInternal::cardknowledge ck, int rank);
    void shift_knowledge(int p_index, int c_index);
    bool update_located_count();
    void wipe_out_playables(Card c);
    int next_discard_index(State s, int player_index);

    void observe_before_move(State s);
    void observe_before_play(State s, move m);
    void observe_before_discard(State s, move m);
    void observe_color_hint(State s, move m);
    void observe_rank_hint(State s, move m);

    move play_lowest_playable(State s);
    move discard_worthless(State s);
    move play_mystery(State s);
    move discard_old(State s);
    std::tuple<move, int> best_hint_for_partner(State s, int partner_index);
    move give_valuable_warning(State s);
    move give_helpful_hint(State s);


public:
    holmesbot(int id, int n_players);
    std::vector<move> get_legal_moves(State s, int id);
    move play(State s);
    void observe(State s, move m);
    int get_id();
    int get_n_cards();
};
