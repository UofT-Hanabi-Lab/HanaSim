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
    
    // parsing arguments ==========================================================================================================================================
    if (argc == 1) {
        std::cout << "Usage:" << std::endl;
        std::cout << "./HanaSim num-games num-players bot1-type ... botn-type --log" << std::endl;
        std::cout << "where n is num-players" << std::endl;
        std::cout << "Options for bot-type: holmes, smart, random, simple, human, tree" << std::endl;
        std::cout << "You can also add a --log flag that will log the moves and scores of every game" << std::endl;
        return 1;
    } else if (argc >= 3) {
        std::string num_games_str;
        for (int i = 0; argv[1][i] != '\0'; i++) {
            num_games_str += argv[1][i];
        }
        // Number of games will be simulated
        num_games = std::stoi(num_games_str);

        std::string num_players_str;
        for (int i = 0; argv[2][i] != '\0'; i++) {
            num_players_str += argv[2][i];
        }
        // Player num, from 2 to 5
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
                if (bot_type != "human" && bot_type != "random" && bot_type != "simple" && bot_type != "holmes" && bot_type != "smart" && bot_type != "tree") {
                    std::cout << "bot-type must be a valid choice (random, holmes, human, simple, smart, or tree)" << std::endl;
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
    // ============================================================================================================================================================
    std::cout << "Welcome to HanaSim!" << std::endl;
    {
        Timer timer;
        int total_score = 0;
        int perfects = 0;

        // sequential loop
        //omp_set_num_threads(1);
        
        for (int i=0; i < num_games; i++) {
            // Create bots instance according to bot_types
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

            // Initialize the game state
            State init_state = State(num_players);
            // Print out all hands if we're logging
            if (log_games) {
                int i = 0;
                for (std::vector<Card> hand : init_state.get_hands()) {
                    std::cout << "Player " << i << "'s Initial Hand: ";
                    for (Card c : hand) {
                        std::cout << c.str() << ", ";
                    }
                    std::cout << std::endl;
                    i++;
                }
            }
            
            game newgame = game(init_state, players);
            int score = newgame.run(log_games);
            total_score += score;
            if (score == 25) perfects++;
            //init_state.reset(); --> for testing with a specific deck (i.e. if you want to see any bots you make are fully deterministic)
        }

        float average_score = (float)total_score / (float)num_games;
        float prop_perf = (float) perfects / (float)num_games;
        std::cout << "Ran " << num_games << " Games with Average Score: " << average_score << std::endl;
        std::cout << perfects << " games with perfect score (" << prop_perf*100 << "%)" << std::endl;

    }
}