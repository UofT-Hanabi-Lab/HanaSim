#include <iostream>


#include "../include/game.h"
#include "../include/randombot.h"
#include "../include/holmesbot.h"
#include "../include/humanplayer.h"


int main()
{
    std::cout << "Welcome to HanaSim!" << std::endl;

    Card newcard(red, five);
    player* p1 = new holmesbot(4, 0, 3); // creating three new random bots with ids 0,1,2 (ids need to be in order starting from 0)
    player* p2 = new holmesbot(4, 1, 3);
    player* p3 = new holmesbot(4, 2, 3);
//    player* p3 = new randombot(4, 2);
//    player* p1 = new humanplayer(4,0);
//    player* p2 = new humanplayer(4,1);
    std::vector<player*> players = {p1, p2, p3};
    State init_state = State(3, 4);
    for (std::vector<Card> hand : init_state.get_hands()) {
        for (Card c : hand) {
            c.str();
        }
    }
    game newgame(init_state, players);

    newgame.run();

}