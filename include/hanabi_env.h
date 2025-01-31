// hanabi_env.h
#pragma once

#include <iostream>
#include <chrono>
#include <string>
#include <vector>
#include <algorithm>
#include <execution>
#include <omp.h>


#include "../include/game.h"

class HanabiEnv {
public:
    HanabiEnv(int num_players);

    Observation reset();

    struct StepResult {
        Observation observation; //
        float reward;          
        bool done;               
        std::string info;       // additional info
    };

    StepResult step(const py::tuple &action);

    void render() const;

private:
    int num_players_;
    bool done_;        // mark if the game end
    State* curr_state_ = NULL; 
    int last_score_;   // To calculate score
    int curr_player_;
};

