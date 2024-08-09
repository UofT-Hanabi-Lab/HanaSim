#include "include/sadagent.h"
#include "../bots/include/smartbot.h"

#include <random>
#include <torch/torch.h>
#include <torch/csrc/autograd/grad_mode.h>

static torch::jit::script::Module torch_jit_load(const std::string &path) {
    auto model = torch::jit::load(path, c10::Device(c10::kCPU));
    return model;
}

torch::jit::script::Module get_torchbot_module(const std::string &path) {
    static thread_local torch::jit::script::Module torchbot_module;
    static thread_local bool torchbot_module_initialized;
    if (!torchbot_module_initialized) {
        torchbot_module = torch_jit_load(path);
        torchbot_module_initialized = true;
    }
    return torchbot_module;
}

std::shared_ptr<AsyncModelWrapper> get_torchbot_async_module(const std::string &path) {
  auto &tp = getThreadPool();
  if (!tp.model) {
    tp.model = std::make_shared<AsyncModelWrapper>(path, "cuda:0", 400);
  }
  return tp.model;
}

TensorDict make_init_hx() {
    static TensorDict init_hx = {
        {"h0", torch::zeros({2, 512})},
        {"c0", torch::zeros({2, 512})},
    };
    return init_hx;
}

void softmax_(float* model_output, std::vector<move> &legal_moves, State s, int id, int num_players, int num_cards) {
    double sum = 0;
    double max_val = -1e9;
    for (auto m: legal_moves) {
        int idx = move_to_index(m, s, id, num_cards, num_players);
        if (model_output[idx] > max_val) {
            max_val = model_output[idx];
        }
    }
    for (auto m: legal_moves) {
        int idx = move_to_index(m, s, id, num_cards, num_players);
        double logit = model_output[idx] - max_val;
        double unnorm = std::exp(logit);
        sum += unnorm;
        model_output[idx] = unnorm;
    }
    sum *= 0.999999; // Don't know why SPARTA has this
    for (auto m: legal_moves) {
        int idx = move_to_index(m, s, id, num_cards, num_players);
        model_output[idx] /= sum;
    }
}

sadagent::sadagent(int n_cards, int id, int n_players, std::string model_name) {
    model_name_ = model_name;
    id_ = id;
    num_players_ = n_players;
    num_cards_ = n_cards;
    if (model_name == "") {
        throw std::runtime_error("Path to model must be specified");
    }

    hx_ = make_init_hx();
}

static std::uniform_real_distribution<double> real_dist(0., 1.);

void sadagent::update_action_probs(at::Tensor model_output, std::vector<move> &legal_moves, int num_moves,  State s) {
    auto out_data = model_output.data_ptr<float>();

    double desired_max_prob = 1 - action_unc_;
    double temperature = 0.5;
    double distance = temperature / 2;
    float temp_data[num_moves];

    for (int iter = 0; iter < 10; iter++) {
        for (int j = 0; j < num_moves; j++) {
            temp_data[j] = out_data[j] / temperature;
        }

        softmax_(temp_data, legal_moves, s, id_, num_players_, num_cards_);
        double max_prob = 0;
        for (auto m : legal_moves) {
            int mi = move_to_index(m, s, id_, num_cards_, num_players_);
            if (temp_data[mi] > max_prob) {
                max_prob = temp_data[mi];
            }
        }
        temperature = (max_prob > desired_max_prob) ?
                        temperature + distance :
                        temperature - distance;
        assert(temperature > 0);
        distance /= 2;
    }
  // clamp the temperature to a reasonable range
    temperature = std::min(std::max(temperature, 0.00001), 1000.);

    // 2. compute action probabilities from a boltzmann distribution
    //    given this temperature

    auto logits = model_output / temperature;
    softmax_(logits.data_ptr<float>(), legal_moves, s, id_, num_cards_, num_players_);

    action_probs_.clear();
    for (auto m : legal_moves) {
        int mi = move_to_index(m, s, id_, num_cards_, num_players_);
        action_probs_[mi] = out_data[mi];
    }
}

void sadagent::observe_before(State s) {
    debug_last_player_ = s.get_active_player();
    debug_last_obs_ = 0;
    if (hand_distribution_v0_.size() == 0) {
        hand_distribution_v0_.reserve(s.get_num_players());
        for (int i = 0; i < s.get_num_players(); i++) {
            hand_distribution_v0_.emplace_back(s, i, num_cards_);
        }
    }
    check_beliefs_(s);
    HleSerializedMove frame = HleSerializedMove(s, id_, last_move_, last_active_card_, last_move_indices_, prev_score_, prev_num_hint_, hand_distribution_v0_);
    auto output = apply_model(frame);

    if (s.get_active_player() == id_) {
        int num_moves = frame.numMoves();
        auto out_data = output.data_ptr<float>();
        std::vector<move> legal_moves = get_legal_moves(s, id_);
        float best_pred = -1e9;
        for (move m : legal_moves) {
            int i = move_to_index(m, s, id_, num_cards_, num_players_);
            if (out_data[i] > best_pred) {
                the_move_ = m;
                best_pred = out_data[i];
            }
        }
        if (action_unc_ > 0) {
            update_action_probs(output, legal_moves, num_moves, s);
        }
    }
}

void sadagent::observe_discard(State s, move m) {
    debug_last_obs_ = 1;
    last_move_ = m;
    last_active_card_ = s.get_hands()[m.get_from()][m.get_card_index()];
    player_about_to_draw_ = m.get_from();
}

void sadagent::observe_play(State s, move m) {
    debug_last_obs_ = 1;
    last_move_ = m;
    last_active_card_ = s.get_hands()[m.get_from()][m.get_card_index()];
    int score = 0;
    for (int i : s.get_piles()) {
        score += i;
    }
    prev_score_ = score;
    prev_num_hint_ = s.get_num_hints();
    player_about_to_draw_ = m.get_from();
}

void sadagent::observe_color_hint(State s, move m) {
    debug_last_obs_ = 1;
    last_move_ = m;
    last_move_indices_ = m.get_card_indices();
    hand_distribution_v0_[m.get_to()].updateFromHint(m, s);
}

void sadagent::observe_rank_hint(State s, move m) {
    debug_last_obs_ = 1;
    last_move_ = m;
    last_move_indices_ = m.get_card_indices();
    hand_distribution_v0_[m.get_to()].updateFromHint(m, s);
}

void sadagent::observe_after(State s) {
    debug_last_obs_ = 2;
    if (player_about_to_draw_ != -1) {
        std::vector<int> deck_count = get_deck_count(s, -1);
        for (int p = 0; p < num_players_; p++) {
            hand_distribution_v0_[p].updateFromRevealedCard(last_active_card_, deck_count, s);
        }
        hand_distribution_v0_[player_about_to_draw_].updateFromDraw(deck_count, last_move_.get_card_index(), s, num_cards_);
        player_about_to_draw_ = -1;
    }
}

torch::Tensor sadagent::apply_model(HleSerializedMove &frame) {
    std::vector<float> frame_vec = frame.toArray();
    auto feats = torch::zeros({(long int) frame_vec.size()});
    auto feats_data = feats.data_ptr<float>();
    for (size_t i = 0; i < frame_vec.size(); i++) {
        if (frame_vec[i] != frame_vec[i]) {
            std::cerr << "input data " << i << " = " << frame_vec[i] << std::endl;
            throw std::runtime_error("Inputs are NaN");
        }
        feats_data[i] = frame_vec[i];
    }

    // std::vector<torch::jit::IValue> inputs;
    // inputs.push_back(feats);
    // inputs.push_back(*hx_);

  // Execute the model and turn its output into a tensor.
    // std::cerr << "Got NN input of size" << feats.sizes() << std::endl;
    // std::vector<torch::jit::IValue> outputs = get_torchbot_module(model_name_).forward(inputs).toTuple()->elements();
    // assert(outputs.size() == 2);
    // at::Tensor output = outputs[0].toTensor();
    // std::cerr << "Got NN output of size " << output.sizes() << std::endl;
    // hx_ = std::make_shared<torch::jit::IValue>(outputs[1]);

    TensorDict input = hx_;
    input["s"] = feats;
    auto output = get_torchbot_async_module(model_name_)->forward(input);
    auto afind = output.find("a");
    auto action = afind->second;
    output.erase(afind);
    hx_ = std::move(output);

    return action;
}

void sadagent::check_beliefs_(const State s) {
  for (int p = 0; p < s.get_num_players(); p++) {
    auto v0 = hand_distribution_v0_[p].get();
    auto true_hand = s.get_hands()[p];
    for (int card_index = 0; card_index < s.get_hands()[p].size(); card_index++) {
      if (v0[card_index][card_to_index(true_hand[card_index])] == 0) {
        throw std::runtime_error("Bad v0 beliefs: " + std::to_string(card_index));
      }
    }
  }
}

void sadagent::observe(State s, move m) {
    observe_before(s);
    if (m.get_type() == PLAY) {
        observe_play(s, m);
    } else if (m.get_type() == DISCARD) {
        observe_discard(s, m);
    } else if (m.get_type() == COL_HINT) {
        observe_color_hint(s, m);
    } else if (m.get_type() == RANK_HINT) {
        observe_rank_hint(s, m);
    }  
}

move sadagent::play(State s) {
    return the_move_;
}

int sadagent::get_id() {
    return id_;
}
int sadagent::get_n_cards() {
    return num_cards_;
}

const std::map<int, float> &sadagent::getActionProbs() const {
    return action_probs_;
}

void sadagent::setActionUncertainty(float action_unc) {
    action_unc_ = action_unc;
}