#pragma once

#include "asyncmodelwrapper.h"
#include <memory>
#include <iostream>
#include "../../bots/include/smartbot.h"
#include "batcher.h"
#include "agentUtils.h"

#include <torch/script.h> // One-stop header.
#include <torch/torch.h>
#include <torch/csrc/autograd/grad_mode.h>


//namespace TorchBotParams {
//
//    const std::string TORCHBOT_MODEL = Params::getParameterString("TORCHBOT_MODEL", "",
//                                                                  "File path to the TorchBot model, saved as serialized TorchScript (required for TorchBot)");
//
//} /* namespace TorchBotParams */


class sadagent final : public player {
    int id_, num_players_, num_cards_;
    std::string model_name_;
    int frame_idx_ = 0;

    // std::shared_ptr<SimulServer> simulserver_;
    // std::shared_ptr<Bot> inner_;
    TensorDict hx_;
    // std::shared_ptr<torch::jit::IValue> hx_;
    std::vector<FactorizedBeliefs> hand_distribution_v0_;

    /* keep track when a partner plays/discards, so that I can update
     * the belief distribution based on their new card */
    int player_about_to_draw_ = -1;
    /* keep track of the last card that I played or discarded
     * to get around server limitations */
    move last_move_ = move(INVALID_MOVE);
    move the_move_ = move(INVALID_MOVE);
    // int last_move_index_;
    std::vector<int> last_move_indices_;
    Card last_active_card_ = Card(red, five);

    int prev_score_ = 0; // server.currentScore();
    int prev_num_hint_ = 0; //server.hintStoneRemaining();

    int debug_last_player_ = 1;
    int debug_last_obs_ = 2;
    at::Tensor apply_model(HleSerializedMove &frame);
    void check_beliefs_(State s);

    std::map<int, float> action_probs_;
    double action_unc_ = 0;
//    std::mt19937 gen_; // FIXME: seed?

    void update_action_probs(
            at::Tensor model_output,
            std::vector<move> &legal_moves,
            int num_moves,
            State s
    );

    void observe_before(State s);
    void observe_discard(State s, move m);
    void observe_play(State s, move m);
    void observe_color_hint(State s, move m);
    void observe_rank_hint(State s, move m);

public:
    sadagent(int n_cards, int id, int n_players, std::string model_name);

    const std::map<int, float> &get_action_probs();
    void set_action_uncertainty(float boltzmann_unc);
    move play(State s);
    void observe(State s, move m);
    void observe_after(State s);
    int get_id();
    int get_n_cards();
    const std::map<int, float> &getActionProbs() const;
    void setActionUncertainty(float boltzmann_unc);
};