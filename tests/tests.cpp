#include <iostream>
#include <chrono>
#include <string>
#include <vector>
#include <algorithm>
#include <execution>
#include <omp.h>
#include <random>
#include <filesystem>

//#include "../include/game.h"
//#include "../bots/include/randombot.h"
//#include "../bots/include/holmesbot.h"
//#include "../bots/include/smartbot.h"
//#include "../include/humanplayer.h"
//#include "../bots/include/simplebot.h"
//#include "../include/matplotlibcpp.h"
#include "../include/global_int.h"

#include "../bots/holmesbot.cpp"
#include "../bots/smartbot.cpp"

#include "../src/card.cpp"
#include "../src/game.cpp"
//
// #include "test_game.cpp"
#include "../src/move.cpp"
#include "../src/state.cpp"
#include "../src/humanplayer.cpp"


//namespace plt = matplotlibcpp;

int seed_num;
int game_num;

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

// move create_move(move_type type, int to_player, int from_player, std::vector<int> card_indices, int color_or_rank){
//     if(int(type) == 1){ // color hint
//         return move(type, to_player, from_player, card_indices, (Color) color_or_rank);
//     }
//     if(int(type) == 2){ // rank hint
//         return move(type, to_player, from_player, card_indices, (Rank) color_or_rank);
//     }
//     if(int(type) == 3 || int(type) == 4){ // play
//         return move(type, from_player, card_indices[0]);
//     }
// }


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


int test_create_two_human_player_custom(){
    player* p1 = new humanplayer(2, 0);
    player* p2 = new humanplayer(2, 1);
    std::vector<player*> players = {};
    players.push_back(p1);
    players.push_back(p2);
    std::vector<Card> deck = test_shuffle_using_seed(494);
    State init_state = State(2, deck);
    game newgame = game(init_state, players);
    std::cout << "tested human player" << std::endl;

    // custom list of moves
    std::vector<int> indices;
    std::vector<move> test_moves = {move((move_type)3, 0, 2), move((move_type)3, 0, 2), 
    move((move_type)3, 1, 2), move((move_type)3, 0, 2), 
    move((move_type)3, 1, 2), move((move_type)3, 0, 2)};

    std::reverse(test_moves.begin(), test_moves.end());
    //for (int i: test_moves) std::cout << i << ' ';
    int score = newgame.run_test(true, test_moves);
    return score;
}

int test_create_two_human_player(){
    player* p1 = new humanplayer(2, 0);
    player* p2 = new humanplayer(2, 1);
    std::vector<player*> players = {};
    players.push_back(p1);
    players.push_back(p2);
    std::vector<Card> deck = test_shuffle_using_seed(494);
    State init_state = State(2, deck);
    game newgame = game(init_state, players);
    std::cout << "tested human player" << std::endl;
    int score = newgame.run(true);
    return score;
}

int test_create_two_holmesbot_custom(){
    player* p1 = new holmesbot(0, 2);
    player* p2 = new holmesbot(1, 2);
    std::vector<player*> players = {};
    players.push_back(p1);
    players.push_back(p2);
    std::vector<Card> deck = test_shuffle_using_seed(494);
    State init_state = State(2, deck);
    game newgame = game(init_state, players);
    std::cout << "tested holmesbot custom" << std::endl;

    // custom list of moves
    std::vector<move> test_moves = {move((move_type)3, 0, 2), move((move_type)3, 1, 2), move((move_type)3, 0, 2), 
    move((move_type)3, 1, 2), move((move_type)3, 0, 2)};

    std::reverse(test_moves.begin(), test_moves.end());
    int score = newgame.run_test(true, test_moves);
    return score;
}

int test_create_two_smartbot_custom(){
    player* p1 = new smartbot(0, 2);
    player* p2 = new smartbot(1, 2);
    std::vector<player*> players = {};
    players.push_back(p1);
    players.push_back(p2);
    std::vector<Card> deck = test_shuffle_using_seed(494);
    State init_state = State(2, deck);
    game newgame = game(init_state, players);
    std::cout << "tested smartbot custom" << std::endl;

    // custom list of moves
    std::vector<move> test_moves = {move((move_type)3, 1, 2), move((move_type)3, 1, 2), move((move_type)3, 0, 2), 
    move((move_type)3, 1, 2), move((move_type)3, 0, 2)};

    std::reverse(test_moves.begin(), test_moves.end());
    int score = newgame.run_test(true, test_moves);
    return score;
}

int test_create_two_smartbot(int seed){
    player* p1 = new smartbot(0, 2);
    player* p2 = new smartbot(1, 2);
    std::vector<player*> players = {};
    players.push_back(p1);
    players.push_back(p2);
    std::vector<Card> deck = test_shuffle_using_seed(seed);
    State init_state = State(2, deck);
    game newgame = game(init_state, players);
    std::cout << "tested smartbot" << std::endl;
    int score = newgame.run(true);
    return score;
}

int main() {
    std::cout << "Tests running :)" << std::endl;
    test_shuffle_using_seed(494);
    seed_num = 494;
    game_num = 1;
    //test_create_two_human_player_custom();
    //test_create_two_holmesbot_custom();
    //test_create_two_smartbot_custom();
    //test_create_two_human_player();
    for (int x = 0; x < 10; x++){
        test_create_two_smartbot(seed_num);
        game_num +=1;
        seed_num +=1; 
    }
    test_create_two_smartbot(seed_num);
    // std::filesystem::remove_all("output/p1");
    return 0;
}

//5 15 15 