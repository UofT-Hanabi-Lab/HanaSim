#include <iostream>
#include <chrono>
#include <string>
#include <vector>
#include <algorithm>
#include <execution>
#include <omp.h>

#include "../include/game.h"
#include "../bots/include/randombot.h"
#include "../bots/include/holmesbot.h"
#include "../bots/include/smartbot.h"
#include "../include/humanplayer.h"
#include "../bots/include/simplebot.h"
//#include "../include/matplotlibcpp.h"

//namespace plt = matplotlibcpp;

class Timer
{
public:
    Timer(){
        m_StartTimepoint = std::chrono::high_resolution_clock ::now();
    }

    ~Timer(){
        Stop();
    }

    void Stop(){
        auto endTimePoint = std::chrono::high_resolution_clock ::now();

        auto start = std::chrono::time_point_cast<std::chrono::microseconds>
                (m_StartTimepoint).time_since_epoch().count();
        auto end = std::chrono::time_point_cast<std::chrono::microseconds>
                (endTimePoint).time_since_epoch().count();

        auto duration = end - start;
        double seconds = duration * 0.000001;

        std::cout << "run duration: " << duration << "us (" << seconds << "s)\n";
    }
private:
    std::chrono::time_point< std::chrono::high_resolution_clock > m_StartTimepoint;
};

int main(int argc, char *argv[])
{
    int num_games = 1;
    int num_players = 2;
    bool log_games = false;
    std::vector<std::string> bot_types = {};
    
    if (argc == 1) {
        std::cout << "Usage:" << std::endl;
        std::cout << "./HanaSim num-games num-players bot1-type ... botn-type --log" << std::endl;
        std::cout << "where n is num-players" << std::endl;
        std::cout << "Options for bot-type: holmes, smart, random, simple, human" << std::endl;
        std::cout << "You can also add a --log flag that will log the moves and scores of every game" << std::endl;
        return 1;
    } else if (argc >= 3) {
        std::string num_games_str;
        for (int i = 0; argv[1][i] != '\0'; i++) {
            num_games_str += argv[1][i];
        }
        num_games = std::stoi(num_games_str);

        std::string num_players_str;
        for (int i = 0; argv[2][i] != '\0'; i++) {
            num_players_str += argv[2][i];
        }
        num_players = std::stoi(num_players_str);
        if (num_players < 2 || num_players > 5 || num_games <= 0) {
            std::cout << "num-games must be positive and num-players must be between 2 and 5" << std::endl;
            return 1;
        }

        std::string log_str;
        for (int i = 0; argv[argc - 1][i] != '\0'; i++) {
            log_str += argv[argc - 1][i];
        }
        if (log_str == "--log") log_games = true;

        if (argc != 3 + num_players + (log_games ? 1 : 0)) {
            std::cout << "Must give num-player bot types" << std::endl;
            return 1;
        } else {
            for (int i = 0; i < num_players; i++) {
                std::string bot_type;
                for (int j = 0; argv[3 + i][j] != '\0'; j++) {
                    bot_type += argv[3 + i][j];
                }
                if (bot_type != "human" && bot_type != "random" && bot_type != "simple" && bot_type != "holmes" && bot_type != "smart") {
                    std::cout << "bot-type must be a valid choice (random, holmes, human, simple, or smart)" << std::endl;
                    return 1;
                }
                bot_types.push_back(bot_type);
            }
        }

        if (argc >= 5) {
            std::string log_str;
            for (int i = 0; argv[4][i] != '\0'; i++) {
                log_str += argv[4][i];
            }
            if (log_str == "--log") log_games = true;
        }
    }
    std::cout << "Welcome to HanaSim!" << std::endl;
    {
        Timer timer;
        int total_score = 0;
        int perfects = 0;

        // sequential loop
//        omp_set_num_threads(1);
        
        //#pragma omp parallel for schedule(static) num_threads(1)
        //State init_state = State(num_players);
        // std::vector<Card> deck = { Card(yellow, three), Card(blue, five), Card(red, three), Card(white, one), Card(green, one), Card(white, four), Card(yellow, one), Card(yellow, one), Card(white, one), Card(red, five), 
        // Card(red, two), Card(blue, four), Card(blue, three), Card(white, two), Card(green, three), Card(blue, one), Card(red, four), Card(white, two), Card(blue, one), Card(red, one), Card(yellow, five), Card(blue, three),
        // Card(green, one), Card(green, four), Card(green, two), Card(red, three), Card(yellow, four), Card(blue, two), Card(green, four), Card(white, four), Card(blue, one), Card(green, two), Card(yellow, two), 
        // Card(green, one), Card(blue, two), Card(green, three), Card(blue, four), Card(yellow, three), Card(white, five), Card(red, one), Card(white, three), Card(yellow, one), Card(white, one), Card(red, one), Card(yellow, two), 
        // Card(red, four), Card(white, three), Card(red, two), Card(yellow, four), Card(green, five)};
        for (int i=0; i < num_games; i++) {
            std::vector<player*> players = {};
            for (int id = 0; id < num_players; id++) {
                player* p;
                if (bot_types[id] == "holmes") {
                    p = new holmesbot(id, num_players);
                } else if (bot_types[id] == "smart") {
                    p = new smartbot(id, num_players);
                } else if (bot_types[id] == "random") {
                    p = new randombot(id, num_players);
                } else if (bot_types[id] == "simple") {
                    p = new simplebot(id, num_players);
                } else if (bot_types[id] == "human") {
                    p = new humanplayer(num_players, id);
                }
                players.push_back(p);
            }

            State init_state = State(num_players);
            if (log_games) {
                for (std::vector<Card> hand : init_state.get_hands()) {
                    for (Card c : hand) {
                        c.str();
                    }
                }
            }

            game newgame = game(init_state, players);
            int score = newgame.run(log_games);
            total_score += score;
            if (score == 25) perfects++;
            //init_state.reset();
        }

        float average_score = (float)total_score / (float)num_games;
        float prop_perf = (float) perfects / (float)num_games;
        std::cout << "Ran " << num_games << " Games with Average Score: " << average_score << std::endl;
        std::cout << perfects << " games with perfect score (" << prop_perf << "%)" << std::endl;

//        omp_set_num_threads(4);
//        #pragma omp parallel for num_threads(4)
//        for (int i = 1; i <= 10; i++) {
//            std::cout << i << std::endl;
//        }
    }
//    plt::plot({1,2,3,4}, "*");
//    plt::show();


}