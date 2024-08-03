#pragma once

#include "asyncmodelwrapper.h"
#include "../../include/player.h"
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
    int me_, numPlayers_, handSize_;
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
    move last_move_;
    move the_move_;
    // int last_move_index_;
    Rank last_move_indices_; // should correspond to HanaSim rank?
    Card last_active_card_ = Card(red, five);

    int prev_score_ = 0; // server.currentScore();
    int prev_num_hint_ = 0; //server.hintStoneRemaining();

    int debug_last_player_ = 1;
    int debug_last_obs_ = 2;
    at::Tensor applyModel(const HleSerializedMove &frame);
    void checkBeliefs_(State s);

    std::map<int, float> action_probs_;
    double action_unc_ = 0;
//    std::mt19937 gen_; // FIXME: seed?

    void updateActionProbs(
            at::Tensor model_output,
            const std::vector<move> &legal_moves,
            int num_moves,
            const State s
    );

public:
    sadagent(int n_cards, int id, int n_players);
    void pleaseObserveBeforeMove(State s);
    void pleaseMakeMove(State s);
    void pleaseObserveBeforeDiscard(State s, move m);
    void pleaseObserveBeforePlay(State s, move m);
    void pleaseObserveColorHint(State s, move m);
    void pleaseObserveValueHint(State s, move m);
    void pleaseObserveAfterMove(State s);
    const std::map<int, float> &getActionProbs();
    void setActionUncertainty(float boltzmann_unc);
    sadagent *clone();
};