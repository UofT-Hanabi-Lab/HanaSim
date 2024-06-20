#include <iostream>


#include "../include/game.h"
#include "../include/randombot.h"
#include "../include/holmesbot.h"
#include "../include/humanplayer.h"


int main()
{
    std::cout << "Welcome to HanaSim!" << std::endl;

    Card newcard(red, five);
    player* p1 = new holmesbot(4, 0, 2); // creating three new random bots with ids 0,1,2 (ids need to be in order starting from 0)
    player* p2 = new holmesbot(4, 1, 2);
//    player* p3 = new randombot(4, 2);
//    player* p1 = new humanplayer(4,0);
//    player* p2 = new humanplayer(4,1);
    std::vector<player*> players = {p1, p2};
    State init_state = State(2, 4);
    game newgame(init_state, players);

    newgame.run();

}