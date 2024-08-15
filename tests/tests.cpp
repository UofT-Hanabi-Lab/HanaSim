#include <iostream>
#include <chrono>
#include <string>
#include <vector>
#include <algorithm>
#include <execution>
//#include <omp.h>
#include "/opt/homebrew/Cellar/libomp/18.1.8/include/omp.h"

#include "../include/game.h"
#include "../bots/include/randombot.h"
#include "../bots/include/holmesbot.h"
#include "../bots/include/smartbot.h"
#include "../include/humanplayer.h"
#include "../bots/include/simplebot.h"
//#include "../include/matplotlibcpp.h"

#include "../bots/holmesbot.cpp"
#include "../src/card.cpp"
#include "../src/game.cpp"
#include "../src/move.cpp"
#include "../src/state.cpp"
#include "../src/humanplayer.cpp"

//namespace plt = matplotlibcpp;

int test_create_two_human_player(){
    player* p1 = new humanplayer(2, 0);
    player* p2 = new humanplayer(2, 1);
    std::vector<player*> players = {};
    players.push_back(p1);
    players.push_back(p2);
    State init_state = State(2);
    game newgame = game(init_state, players);
    std::cout << "tested human player" << std::endl;
    int score = newgame.run(true);
}

int main() {

    std::cout << "Tests running :)" << std::endl;
    test_create_two_human_player();
    return 0;
}
