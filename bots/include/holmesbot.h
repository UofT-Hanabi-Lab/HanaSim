#include <tuple>
#include <iostream>

#include "../../include/player.h"


#pragma once

class holmesbot;

namespace HolmesBotInternal {

class holmes_cardknowledge {
    bool cant_be_[6][6]; // boolean array to show what kind of card it can't be
    Color color_;
    Rank rank_;
public:
    holmes_cardknowledge();

    // it <func_name> this color/rank/card?
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
    friend class HolmesBotInternal::holmes_cardknowledge;

    int num_cards_;
    int id_;
    int located_count_[6][6]; // how many of this type of card can I locate on people's hands
    int played_count_[6][6]; // hoe many of this card has been played/discarded
    std::vector<std::vector<HolmesBotInternal::holmes_cardknowledge>> hand_knowledge_; // what each player knows about their hand
    int lowest_playable_rank_;
    int my_hand_size_; // can change depending on whether you're in the 'last round' or not

    bool is_valuable(State s, Card card) const;
    bool could_be_valuable(State s, HolmesBotInternal::holmes_cardknowledge ck, int rank);
    void shift_knowledge(int p_index, int c_index); // for when you draw a new card
    bool update_located_count();
    void wipe_out_playables(Card c); // called if this type of card has been successfully played, so all other cards of this type are no longer playable
    int next_discard_index(State s, int player_index); // the index of your next discard. -1 if you're not discarding.

    // for the observe function
    void observe_before_play(State s, move m);
    void observe_before_discard(State s, move m);
    void observe_color_hint(State s, move m);
    void observe_rank_hint(State s, move m);


    // for the playe function
    move play_lowest_playable(State s);
    move discard_worthless(State s);
    move play_mystery(State s);
    move discard_old(State s);
    std::tuple<move, int> best_hint_for_partner(State s, int partner_index);
    move give_valuable_warning(State s);
    move give_helpful_hint(State s);


public:
    holmesbot(int id, int n_players);
    move play(State s);
    move play_prechosen(State s, move chosen_m);
    void observe_before_move(State s);
    void observe(State s, move m);

    int get_id();
    int get_n_cards();
    std::vector<std::vector<HolmesBotInternal::holmes_cardknowledge>> get_hk();
};
