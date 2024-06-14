#include "player.h"


class game {
    std::vector<player*> players_;
    State curr_state_;

public:
    game(State init_state, std::vector<player*>);
    void run();
    State get_curr_state();
};

