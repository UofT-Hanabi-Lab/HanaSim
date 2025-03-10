#include "../../include/player.h"
#include <cstdint>
#include <tuple>


#pragma once

class smartbot;


namespace SmartBotInternal {


enum trivalue : int8_t {
    NO, MAYBE, YES
};

// check holmes_cardknowledge if confused about some functions/attributes
class smart_cardknowledge {
    smartbot *bot_;
    bool cant_be_[6][6];  // A 6x6 matrix of booleans tracking which (color,rank) combinations are impossible
    int color_;  // if known, the deduced color, otherwise it's set to -1 or -2
    int rank_;  // if known, the deduced rank, otherwise it's set to -1 or -2
    int possibilities_; // how many color-rank configurations are possible for this card
    trivalue playable_;
    trivalue valuable_;
    trivalue worthless_;
    float prob_playable_;
    float prob_valuable_;
    float prob_worthless_;
public:
    smart_cardknowledge(smartbot *bot);


    bool must_be(Color color);
    bool must_be(Rank rank);
    bool cannot_be(Card card);
    bool cannot_be(Color color);
    bool cannot_be(Rank rank);
    int color() { compute_identity(); return color_; };
    int rank() { compute_identity(); return rank_; };
    bool known() { compute_identity(); return color_ != -1 && rank_ != -1; }


    void set_must_be(Color color);
    void set_must_be(Rank rank);
    void set_must_be(Card card);
    void set_cannot_be(Color color);
    void set_cannot_be(Rank rank);
    void set_is_playable(bool playable, State s);
    void set_is_valuable(bool valuable, State s);
    void set_is_worthless(bool worthless, State s);
    void befuddle_by_discard();
    void befuddle_by_play(bool success);


    void update(bool use_eyesight); // update cant_be_


   
    Card known_card() { return Card(Color(color_), Rank(rank_)); }


    int possibilities() { compute_possibilities(); return possibilities_; }


    trivalue playable(State s) { compute_playable(s); return playable_; }
    trivalue valuable(State s) { compute_valuable(s); return valuable_; }
    trivalue worthless(State s) { compute_worthless(s); return worthless_; }
    trivalue get_playable() { return playable_; }
    trivalue get_valuable() { return valuable_; }
    trivalue get_worthless() { return worthless_; }


    float prob_playable(State s) { compute_playable(s); return prob_playable_; }
    float prob_valuable(State s) { compute_valuable(s); return prob_valuable_; }
    float prob_worthless(State s) { compute_worthless(s); return prob_worthless_; }


    bool could_be_playable(int rank, State s);
    bool could_be_valuable(int rank, State s);


    void compute_identity();
    void compute_possibilities();
    void compute_playable(State s);
    void compute_valuable(State s);
    void compute_worthless(State s);
};
}


class smartbot final : public player {
    friend class SmartBotInternal::smart_cardknowledge;
    int id_;
    int num_cards_;
    std::vector<std::vector<SmartBotInternal::smart_cardknowledge>> hand_knowledge_;  // what each player knows about their hand
    int located_count_[6][6]; // cards that have been definitely identified
    int played_count_[6][6]; // cards that have been played/discarded
    int eyesight_count_[6][6]; // cards visible to me in particular


    bool is_valuable(State s, Card card);
    bool is_playable(State s, Card card);
    bool is_worthless(State s, Card card);
    void shift_knowledge(int p_index, int c_index, bool draw);
    bool update_located_count();
    void update_eyesight_count(State s);
    int next_discard_index(State s, int player_index);
    void no_warning_given(int from, State s); // called if this partner hasn't given a warning to his most immediately next partner


    void observe_before_play(State s, move m);
    void observe_before_discard(State s, move m);
    void observe_color_hint(State s, move m);
    void observe_rank_hint(State s, move m);


    move discard_finesse(State s);
    move play_lowest_playable(State s);
    move discard_worthless(State s);
    move play_mystery(State s);
    move discard_old(State s);


    template<class F>
    std::tuple<move, int> best_hint_for_partner_given_constraint(State s, int partner_index, F&& is_okay);

    std::tuple<move, int> best_hint_for_partner(State s, int partner_index);
    move give_valuable_warning(State s);
    move give_helpful_hint(State s);


public:
    smartbot(int id, int n_players);
    std::vector<move> get_legal_moves(State s, int id);
    move play(State s);
    move play_prechosen(State s, move chosen_m);
    void observe(State s, move m);
    void observe_before_move(State s);
    int get_id();
    int get_n_cards();
    std::vector<std::vector<SmartBotInternal::smart_cardknowledge>> get_hk();
};
