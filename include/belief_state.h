#pragma once
#include "state.h"
#include <map>
#include <utility>

/*NOTE: currently we only implement single agent SPARTA search, where only one
agent performs search. Therefore, we only need private belief. No need public belief.

Definition:
    Private Belief: based on the player1's AOH, what is the probability distribution
        of the state(or more generally: trajectory).
        In tuitively, how the player think about the real world.

To implemented multi-agent SPARTA search: need public belief.
Definition:
    Public Belief: based on all players' common knowledge, what is the probability 
        distribution of the trajectory.
        Intuitively, how all the players think about the real world.
    Common Knowledge: cards already played/hinted/discard... all info that everyone knows
*/
class BeliefState : public State {
public:
    BeliefState(int num_players);

    std::vector<std::map<std::pair<Color, Rank>, double>> belief_hand_;

    void update_belief_from_action(const move& m);
    void apply_hint(int index, Color color);
    std::vector<Card> sample_self_hand() const;

    // Indexed by hand index (0..cards_per_hand_-1), each is a map from (color, rank) → probability
    std::vector<std::map<std::pair<Color, Rank>, double>> belief_hand_;

private:
    void initialize_uniform_belief();
};
