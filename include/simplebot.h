#include <tuple>
#include <iostream>

#include "player.h"


#pragma once

class simplebot;

namespace SimpleBotInternal {

struct cardknowledge {
   bool cantBe_[6][6];
   Color color_;
   Rank rank_;
public:
   cardknowledge();

   bool must_be(Color color);
   bool must_be(Rank rank);
   bool cannot_be(Card card);
   bool cannot_be(Color color);
   bool cannot_be(Rank rank);
   int rank();
   int color();

   void set_must_be(Color color);
   void set_must_be(Rank rank);
   void set_cannot_be(Color color);
   void set_cannot_be(Rank rank);


   void update(State s, const simplebot &bot);

   bool is_playable;

//private:
//    enum Possibility { NO, MAYBE, YES };
//    Possibility colors_[Hanabi::NUMCOLORS];
//    Possibility values_[5+1];
};
}

class simplebot final : public player {
   int id_;
   int my_hand_size_;
   int num_cards_;
   int located_count_[6][6];
   int lowest_playable_rank_;

   /* What does each player know about his own hand? */
   std::vector<std::vector<SimpleBotInternal::cardknowledge>> hand_knowledge_;
   int played_count_[6][6];
//    void shift_knowledge(int p_index, int c_index);
   void invalidateKnol(int p_index, int c_index);
   void wipeOutPlayables(Card c);
   void observe_before_move(State s);
   void observe_before_play(State s, move m);
   void observe_before_discard(State s, move m);
   void observe_color_hint(State s, move m);
   void observe_rank_hint(State s, move m);
   bool update_located_count();

   move play_lowest_playable(State s);
   std::tuple<move, int> best_hint_for_partner(State s, int partner_index);
   move give_helpful_hint(State s);
   move discard_old(State s);
   move play_mystery(State s);

public:
   simplebot(int n_cards, int id, int n_players);
   std::vector<move> get_legal_moves(State s, int id);
   move play(State s);
   void observe(State s, move m);
   int get_id();
   int get_n_cards();
};