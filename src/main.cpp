#include <iostream>
#include <chrono>
#include <string>
#include <vector>
#include <algorithm>
#include <execution>
#include <omp.h>

#include "../include/game.h"
#include "../include/randombot.h"
#include "../include/holmesbot.h"
#include "../include/smartbot.h"
#include "../include/humanplayer.h"
#include "../include/simplebot.h"
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
    std::string bot_type;
    bool log_games = false;
    
    if (argc == 1) {
        std::cout << "Usage:" << std::endl;
        std::cout << "./HanaSim num-games num-players bot-type --log" << std::endl;
        std::cout << "Options for bot-type: holmes, smart, random, simple, human" << std::endl;
        std::cout << "You can also add a --log flag that will log the moves and scores of every game" << std::endl;
        return 1;
    } else if (argc >= 4) {\
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

        for (int i = 0; argv[3][i] != '\0'; i++) {
            bot_type += argv[3][i];
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
        #pragma omp parallel for schedule(static) num_threads(1)
        for (int i=0; i < num_games; i++) {
            std::vector<player*> players = {};
            for (int id = 0; id < num_players; id++) {
                player* p;
                if (bot_type == "holmes") {
                    p = new holmesbot(4, id, num_players);
                } else if (bot_type == "smart") {
                    p = new smartbot(4, id, num_players);
                } else if (bot_type == "random") {
                    p = new randombot(4, id);
                } else if (bot_type == "simple") {
                    p = new simplebot(4, id, num_players);
                } else if (bot_type == "human") {
                    p = new humanplayer(4, id);
                }
                players.push_back(p);
            }

            State init_state = State(num_players, 4);
            if (log_games) {
                for (std::vector<Card> hand : init_state.get_hands()) {
                    for (Card c : hand) {
                        c.str();
                    }
                }
            }

            game newgame(init_state, players);
            int score = newgame.run(log_games);
            total_score += score;
            if (score == 25) perfects++;
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