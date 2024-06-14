#include <iostream>


#include "../include/game.h"
#include "../include/randombot.h"
#include "../include/humanplayer.h"


int main()
{
    std::cout << "Welcome to HanaSim!" << std::endl;

    Card newcard(red, five);
//    player* p1 = new randombot(4, 0); // creating three new random bots with ids 0,1,2 (ids need to be in order starting from 0)
//    player* p2 = new randombot(4, 1);
//    player* p3 = new randombot(4, 2);
    player* p1 = new humanplayer(4,0);
    player* p2 = new humanplayer(4,0);
    std::vector<player*> players = {p1, p2};
    State init_state(2, 4); // 3 players, 4 cards per player
    game newgame(init_state, players);

    newgame.run();

}