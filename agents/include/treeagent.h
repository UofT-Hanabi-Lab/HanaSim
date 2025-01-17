#include "../../include/player.h"
#include <string>


class treeagent : public player {
    int num_cards_;
    int id_;
    int num_players_;
    player *partner_;
    //supul_t tree_;

public:
    treeagent(int id, int num_players, std::string sample_path, player *partner);
    ~treeagent();
    move play(State s);
    move play_prechosen(State s, move chosen_m);
    void observe(State s, move m);
    void observe_before_move(State s);
    int get_id();
    int get_n_cards();
    void insert_and_update(std::string path_to_csv); // insert the data in the csv and update tree metadata
    void rebuild(); // rebuild the tree to get more accurate "leaves"
};