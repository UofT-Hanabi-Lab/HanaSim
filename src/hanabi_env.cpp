#include <iostream>

#include "../include/hanabi_env.h"
#include "../bots/include/randombot.h"
#include "../bots/include/holmesbot.h"
#include "../bots/include/smartbot.h"

HanabiEnv::HanabiEnv(int num_players)
    : num_players_(num_players), done_(false), last_score_(0), players_({})
{
    curr_state_ = new State(num_players);
    last_score_ = 0;
    curr_player_id_ = 0;
}

Observation HanabiEnv::reset()
{
    done_ = false;
    curr_state_->reset();
    last_score_ = 0;
    curr_player_id_ = 0;
    // for (int top : curr_state_->get_piles()) {
    //     score += top;
    // }
    // last_score_ = score;

    return curr_state_->get_observation(0);
}

// Decode the python tuple to move object
move decode_move(const py::tuple &t)
{
    if (t.size() != 7)
    {
        throw std::runtime_error("decode_move: expected a 7-element tuple");
    }

    move_type type = static_cast<move_type>(t[0].cast<int>());
    int to = t[1].cast<int>();
    int from = t[2].cast<int>();
    int card_index = t[3].cast<int>();

    std::vector<int> card_indices;
    py::list py_card_indices = t[4].cast<py::list>();
    for (auto item : py_card_indices)
    {
        card_indices.push_back(item.cast<int>());
    }

    Color color = static_cast<Color>(t[5].cast<int>());
    Rank rank = static_cast<Rank>(t[6].cast<int>());

    switch (type)
    {
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

HanabiEnv::StepResult HanabiEnv::step(const std::optional<py::tuple> maybe_action)
{
    move next_move = move(move_type::INVALID_MOVE);
    if (players_.find(curr_player_id_) != players_.end())
    {
        // case 1: current player is registered in the environment

        const auto curr_player = players_.at(curr_player_id_);

        if (maybe_action.has_value())
        {
            throw std::invalid_argument("Expected action to be null, it's an internal player's turn");
        }

        for (const auto &[_, p] : players_)
        {
            p->observe_before_move(*curr_state_);
        }

        /******* BEGIN: Create handknowledge (only for smartbot and holmesbot) ********/

        const int num_cards_ = (num_players_ <= 3) ? 5 : 4;
        std::vector<int> hk_playable = {};
        std::vector<int> hk_valuable = {};
        std::vector<int> hk_worthless = {};
        std::vector<int> hk_cols = {};
        std::vector<int> hk_ranks = {};

        if (const auto smart_partner = std::dynamic_pointer_cast<smartbot>(curr_player))
        {
            // Generate knowledge for smartbot

            std::vector<std::vector<SmartBotInternal::smart_cardknowledge>> hk = smart_partner->get_hk();
            for (int i = 0; i < hk.size(); i++)
            {
                for (int j = 0; j < hk[i].size(); j++)
                {
                    hk_playable.push_back((hk[i][j].playable(*curr_state_) == SmartBotInternal::NO) ? 0 : ((hk[i][j].get_playable() == SmartBotInternal::YES) ? 1 : 2));
                    hk_valuable.push_back((hk[i][j].valuable(*curr_state_) == SmartBotInternal::NO) ? 0 : ((hk[i][j].get_valuable() == SmartBotInternal::YES) ? 1 : 2));
                    hk_worthless.push_back((hk[i][j].worthless(*curr_state_) == SmartBotInternal::NO) ? 0 : ((hk[i][j].get_worthless() == SmartBotInternal::YES) ? 1 : 2));
                    hk_cols.push_back((hk[i][j].color() < 0) ? 6 : hk[i][j].color());
                    hk_ranks.push_back((hk[i][j].rank() < 0) ? 6 : hk[i][j].rank());
                }
            }
        }
        else if (const auto holmes_partner = std::dynamic_pointer_cast<holmesbot>(curr_player))
        {
            // Generate knowledge for holmesbot

            const std::vector<std::vector<HolmesBotInternal::holmes_cardknowledge>> hk = holmes_partner->get_hk();
            for (int i = 0; i < hk.size(); i++)
            {
                for (int j = 0; j < hk[i].size(); j++)
                {
                    hk_playable.push_back((int)hk[i][j].is_playable);
                    hk_valuable.push_back((int)hk[i][j].is_valuable);
                    hk_worthless.push_back((int)hk[i][j].is_worthless);
                    hk_cols.push_back((int)(hk[i][j].color()));
                    hk_ranks.push_back((int)(hk[i][j].rank()));
                }
            }
        }

        /******** END: Create handknowledge (only for smartbot and holmesbot) *********/

        next_move = curr_player->play(*curr_state_);
    }
    else
    {
        // case 2: current player is not registered in the environment,
        //     we're receiving an action

        if (!maybe_action.has_value())
        {
            throw std::invalid_argument("Expected action to be non-null, it's an external player's turn");
        }

        next_move = decode_move(*maybe_action);
    }

    // Notify all internal players of the move
    for (const auto &[_, p] : players_)
    {
        p->observe(*curr_state_, next_move);
    }

    // Update the game state based on the move
    curr_state_->transition(next_move, false);

    // Update the score
    int score = 0;
    for (int top : curr_state_->get_piles())
    { // Tabulate scores
        score += top;
    }
    last_score_ = score;

    // Set done_ if the game is over
    done_ = (last_score_ >= 25) || (curr_state_->get_num_lives() == 0) || curr_state_->get_deck().empty();

    curr_player_id_ = (curr_player_id_ + 1) % num_players_;

    return {
        /* observation = */ curr_state_->get_observation(curr_player_id_), // In observation will return valid move for next player
        /* reward = */ 1.0f,                                               // need calculate rewards here
        /* done = */ done_,
        /* info = */ std::to_string(last_score_),
        /* last_move = */ next_move.as_tuple()};
}

void HanabiEnv::render() const
{
    // Print Hand. Better update to return an object and visualize in python

    int i = 0;
    for (std::vector<Card> hand : curr_state_->get_hands())
    {
        std::cout << "Player " << i << "'s Initial Hand: ";
        for (Card c : hand)
        {
            std::cout << c.str() << ", ";
        }
        std::cout << std::endl;
        i++;
    }
}

void HanabiEnv::add_player(const PlayerName name, const int player_id)
{
    switch (name)
    {
    case PlayerName::RandomBot:
        players_[player_id] = std::make_shared<randombot>(player_id, num_players_);
        break;

    case PlayerName::HolmesBot:
        players_[player_id] = std::make_shared<holmesbot>(player_id, num_players_);
        break;

    case PlayerName::SmartBot:
        players_[player_id] = std::make_shared<smartbot>(player_id, num_players_);
        break;

    default:
        throw std::invalid_argument("Unsupported player name");
    }
}

std::vector<PlayerName> HanabiEnv::get_supported_player_names() const
{
    return {PlayerName::RandomBot, PlayerName::HolmesBot, PlayerName::SmartBot};
}
