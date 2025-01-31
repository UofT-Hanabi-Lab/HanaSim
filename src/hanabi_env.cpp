#include "../include/hanabi_env.h"
#include <iostream>

HanabiEnv::HanabiEnv(int num_players)
: num_players_(num_players), done_(false), last_score_(0)
{
    curr_state_ = new State(num_players);
    last_score_ = 0;
    curr_player_ = 0;
}

Observation HanabiEnv::reset() {
    done_ = false;
    curr_state_->reset();
    last_score_ = 0;
    curr_player_ = 0;
    // for (int top : curr_state_->get_piles()) {
    //     score += top;
    // }
    // last_score_ = score;


    return curr_state_->get_observation(0);
}

// Decode the python tuple to move object
move decode_move(const py::tuple &t) {
    if (t.size() != 7) {
        throw std::runtime_error("decode_move: expected a 7-element tuple");
    }

    move_type type = static_cast<move_type>(t[0].cast<int>());
    int to = t[1].cast<int>();
    int from = t[2].cast<int>();
    int card_index = t[3].cast<int>();

    std::vector<int> card_indices;
    py::list py_card_indices = t[4].cast<py::list>();
    for (auto item : py_card_indices) {
        card_indices.push_back(item.cast<int>());
    }

    Color color = static_cast<Color>(t[5].cast<int>());
    Rank rank = static_cast<Rank>(t[6].cast<int>());

    switch (type) {
        case PLAY:
        case DISCARD:
            return move(type, from, card_index);

        case COL_HINT:
            return move(type, to, from, card_indices, color);

        case RANK_HINT:
            return move(type, to, from, card_indices, rank);

        default:
            return move(INVALID_MOVE); 
    }
}


HanabiEnv::StepResult HanabiEnv::step(const py::tuple &action) {
    move m = decode_move(action);
    printf("c++ received action: %i", m.get_type());
    curr_state_->transition(m,true);
    int score = 0;
    for (int top : curr_state_->get_piles()) { // Tabulate scores
        score += top;
    }
    last_score_ = score;
    done_ = !((last_score_ < 25) && (curr_state_->get_num_lives() > 0) && !(curr_state_->get_deck().empty()));
    curr_player_ = ((curr_player_+1)%num_players_);
    return {
        /* observation = */ curr_state_->get_observation(curr_player_),    // In observation will return valid move for next player
        /* reward = */ 1.0f,            // need calculate rewards here
        /* done = */ done_,         
        /* info = */ std::to_string(last_score_)
    };
}

void HanabiEnv::render() const {
    // Print Hand. Better update to return an object and visualize in python

    int i = 0;
    for (std::vector<Card> hand : curr_state_->get_hands()) {
        std::cout << "Player " << i << "'s Initial Hand: ";
        for (Card c : hand) {
            std::cout << c.str() << ", ";
        }
        std::cout << std::endl;
        i++;
    }
}