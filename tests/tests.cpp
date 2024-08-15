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

//namespace plt = matplotlibcpp;

int test_create_two_holmes_player(){
    player* p1 = new holmesbot(0, 2);
    player* p2 = new holmesbot(1, 2);
    std::cout << "tested holmesbot" << std::endl;
}

int main() {

    std::cout << "Tests running :)" << std::endl;
    test_create_two_holmes_player();
    return 0;
}
