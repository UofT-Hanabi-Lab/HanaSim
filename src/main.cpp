#include <iostream>
#include <chrono>
#include <string>
#include <execution>
//#include <omp.h> // brew install libomp

#include "../include/game.h"
#include "../include/randombot.h"
#include "../include/holmesbot.h"
#include "../include/humanplayer.h"
#include "../include/simplebot.h"

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
    std::string num;

    if (argc == 2) {
        int i = 0;
        for (i = 0; argv[1][i] != '\0'; i++) {
            num += argv[1][i];
        }
        num_games = std::stoi(num);
    }
    std::cout << "Welcome to HanaSim!" << std::endl;
    {
        Timer timer;
        int total_score = 0;

        // sequential loop
        for (int i=0; i < num_games; ++i){
//                    player* p1 = new simplebot(4,0,3);
//        player* p2 = new simplebot(4,1,3);
//        player* p3 = new simplebot(4,2,3);
            player* p1 = new holmesbot(4, 0, 3); // creating three new random bots with ids 0,1,2 (ids need to be in order starting from 0)
            player* p2 = new holmesbot(4, 1, 3);
            player* p3 = new holmesbot(4, 2, 3);
//        player* p3 = new randombot(4, 2);
//        player* p1 = new humanplayer(4,0);
//        player* p2 = new humanplayer(4,1);
            std::vector<player*> players = {p1, p2, p3};
            State init_state = State(3, 4);
            for (std::vector<Card> hand : init_state.get_hands()) {
                for (Card c : hand) {
                    c.str();
                }
            }
            game newgame(init_state, players);

            total_score += newgame.run();
        }
        float average_score = (float)total_score / (float)num_games;
        std::cout << "Ran " << num_games << " Games with Average Score: " << average_score << std::endl;
    }



}