#include <iostream>
#include <chrono>
#include <string>
#include <vector>
#include <algorithm>
#include <execution>
#include <omp.h>
#include <random>

//#include "../include/game.h"
//#include "../bots/include/randombot.h"
//#include "../bots/include/holmesbot.h"
//#include "../bots/include/smartbot.h"
//#include "../include/humanplayer.h"
//#include "../bots/include/simplebot.h"
//#include "../include/matplotlibcpp.h"

//#include "../bots/holmesbot.cpp"
#include "../src/card.cpp"
//#include "../src/game.cpp"
#include "test_game.cpp"
#include "../src/move.cpp"
#include "../src/state.cpp"
#include "../src/humanplayer.cpp"


//namespace plt = matplotlibcpp;

void print_card(Card c){
  	if(c.color()== 1){
          std::cout<< "red " << c.rank() << std::endl;
  	}
    if(c.color()== 2){
          std::cout<< "white " << c.rank() << std::endl;
  	}
    if(c.color()== 3){
          std::cout<< "yellow " << c.rank() << std::endl;
  	}
    if(c.color()== 4){
          std::cout<< "green " << c.rank() << std::endl;
  	}
    if(c.color()== 5){
          std::cout<< "blue " << c.rank() << std::endl;
  	}
}


std::vector<Card> deck_ = { Card(red, one), Card(red, one), Card(red, one), Card(red, two), Card(red, two), Card(red, three), Card(red, three), Card(red, four), Card(red, four), Card(red, five),
              Card(blue, one), Card(blue, one), Card(blue, one), Card(blue, two), Card(blue, two), Card(blue, three), Card(blue, three), Card(blue, four), Card(blue, four), Card(blue, five),
              Card(yellow, one), Card(yellow, one), Card(yellow, one), Card(yellow, two), Card(yellow, two), Card(yellow, three), Card(yellow, three), Card(yellow, four), Card(yellow, four), Card(yellow, five),
              Card(green, one), Card(green, one), Card(green, one), Card(green, two), Card(green, two), Card(green, three), Card(green, three), Card(green, four), Card(green, four), Card(green, five),
              Card(white, one), Card(white, one), Card(white, one), Card(white, two), Card(white, two), Card(white, three), Card(white, three), Card(white, four), Card(white, four), Card(white, five)};

std::vector<Card> test_shuffle_using_seed(int seed){
	std::vector<Card> new_deck = deck_;
	std::default_random_engine rng(seed);
    std::shuffle(new_deck.begin(), new_deck.end(), rng);
    for (int x = 0; x < new_deck.size(); x++) {
        print_card(new_deck[x]);
    }
    return new_deck;
}


int test_create_two_human_player(){
    player* p1 = new humanplayer(2, 0);
    player* p2 = new humanplayer(2, 1);
    std::vector<player*> players = {};
    players.push_back(p1);
    players.push_back(p2);
    std::vector<Card> deck = test_shuffle_using_seed(494);
    State init_state = State(2, deck);
    test_game newgame = test_game(init_state, players);
    std::cout << "tested human player" << std::endl;
    std::vector<int> test_moves;
    int score = newgame.run(true, test_moves);
    return score;
    return 0;
}

int main() {

    std::cout << "Tests running :)" << std::endl;
    test_shuffle_using_seed(494);
    test_create_two_human_player();
    return 0;
}
