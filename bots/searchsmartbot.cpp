#include "include/searchsmartbot.h"
#include "player.h"        
#include "smartbot.h"
#include <memory>

//TODO: currently, search simulation happens when it's acting agent's turn.
//Improvements:
//      May add an upperbound of search space, and enables larger amount of simulations.
//      if (sizeof(player1.search_space) > threshold) 
//          {act according to blueprint policy (eg. smartbot)}

spartabot::spartabot(int id, int n_players, int search_n, float search_thresh)
    : id_(id), n_players_(n_players), search_n_(search_n), 
    search_thresh_(search_thresh) {
    bp_ = std::make_shared<smartbot>(id, n_players);
}

void spartabot::initialize_belief(const State& s){
    bs_ = BeliefState(s, id_);
}

void spartabot::observe_before_move(State s){
    bp_->observe_before_move(s);
}
move spartabot::play(State s) {

    //only replace the move with search when search move score is higher than 
    // bluprint score by search_thresh_ points
    move bp_move = bp_->play(s);

    std::vector<move> legal_moves = s.get_legal_moves(id_);
    move best_move = bp_move;
    float best_score = -1;

    // Note that if the transition function is only stocastic when you shuffle the deck every time
    // here we asssume no shuffling, so the bp_score is fixed given the start state.
    State s_copy = s;
    s_copy.transition(bp_move, false);
    float bp_score = simulate_game(s_copy);

    for (const move& m : legal_moves){

        float score_sofar = 0;
        for (int i=0; i<search_n_;i++){
            BeliefState sample_state = bs_.sample_belief_state();
            //TODO: implement the belief distribution function
            //TODO: not the real state, but generate the state based on distribution of real state.
            sample_state.transition(m, false);
            score_sofar += simulate_game(sample_state);
        }
        float avg = score_sofar / search_n_;

        if (avg>best_score){
            best_move = m;
            best_score = avg;
        }
    }

    if (best_score - bp_score > search_thresh_){
        return best_move;
    }

    return bp_move;
}

void spartabot::observe(State s, move m) {
    bp_->observe(s, m);
    bs_.update_belief(s, m);
}

move spartabot::play_prechosen(State s, move m) {
    return bp_->play_prechosen(s, m);
}

int spartabot::get_id() {
    return id_;
}

int spartabot::get_n_cards() {
    return bp_->get_n_cards();
}

float spartabot::simulate_game(State s) {
    // Keep simulating until terminal condition is reached
    while (s.get_num_lives() > 0 && !s.get_deck().empty()) {
        int current_player = s.get_deck().size() % n_players_;  // estimation of who's turn it is
        std::vector<move> legal_moves = s.get_legal_moves(current_player);
        
        if (legal_moves.empty()) break;

        // Use blueprint policy to choose a move
        move m = bp_->play(s);
        s.transition(m, false);  // false = no logging
    }

    // Score = sum of top cards in each pile (ignoring index 0 if that's a placeholder)
    std::vector<int> piles = s.get_piles();
    int score = 0;
    for (size_t i = 1; i < piles.size(); ++i) {
        score += piles[i];
    }
    return static_cast<float>(score);
}


/*backup*/



// move spartabot::sparta_play(BeliefState s){
//     //only replace the move with search when search move score is higher than 
//     // bluprint score by search_thresh_ points
//     move bp_move = bp_->play(s);

//     std::vector<move> legal_moves = s.get_legal_moves(id_);
//     move best_move = bp_move;
//     float best_score = -1;

//     // Note that if the transition function is only stocastic when you shuffle the deck every time
//     // here we asssume no shuffling, so the bp_score is fixed given the start state.
//     State s_copy = s;
//     s_copy.transition(bp_move, false);
//     float bp_score = simulate_game(s_copy);

//      for (const move& m : legal_moves){

//         float score_sofar = 0;
//         for (int i=0; i<search_n_;i++){
//             BeliefState sample_state = s.sample_belief_state();
//             //TODO: implement the belief distribution function
//             //TODO: not the real state, but generate the state based on distribution of real state.
//             sample_state.transition(m, false);
//             score_sofar += simulate_game(sample_state);
//         }
//         float avg = score_sofar / search_n_;

//         if (avg>best_score){
//             best_move = m;
//             best_score = avg;
//         }
//     }

//     if (best_score - bp_score > search_thresh_){
//         return best_move;
//     }

//     return bp_move;
// }