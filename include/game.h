#include "player.h"
#pragma once

// The game class manages the overall game state, players, and game mechanics.
class game {
    // Vector of player pointers to store the players participating in the game
    std::vector<player*> players_;
    // The current state of the game
    State curr_state_;

public:
    // Constructor: Initializes the game with a starting state and a list of players
    game(State init_state, std::vector<player*>);
    // Runs the main game loop
    // Parameters:
    // - log_game: If true, logs the game details to the console
    // Returns: Final game score
    int run(bool log_game);

    //int create_hand_knowledge(std::vector<player*> players, int curr_player);

    // Writes details of a move to a CSV file
    // Parameters:
    // - file: Output file stream for the CSV file
    // - move: The move to write
    // Returns: 0 on success
    int write_move_csv(std::ofstream &file, move move);

    // Writes the current hands of all players to a CSV file
    // Parameters:
    // - file: Output file stream for the CSV file
    // - s: Current game state
    // - curr_p: The current player index
    // - num_p: The total number of players
    // Returns: 0 on success
    int write_hand_csv(std::ofstream &file, State s, int curr_p, int num_p);

    // Writes the current state of the piles to a CSV file
    // Parameters:
    // - file: Output file stream for the CSV file
    // - s: Current game state
    // Returns: 0 on success
    int write_pile_csv(std::ofstream &file, State s);

    // Writes the deck and discard pile information to a CSV file
    // Parameters:
    // - file: Output file stream for the CSV file
    // - s: Current game state
    // Returns: 0 on success
    int write_deck_and_discards_csv(std::ofstream &file, State s);

    // Combines multiple CSV files into a single file for game logging
    // Parameters:
    // - out: Output file stream for the combined CSV file
    // - in1, in2, in3, in4: Paths to input CSV files for deck, piles, hands, and moves
    // - turn: Current turn number
    // Returns: 0 on success
    int combine_csv(std::ofstream &out, std::string in1, std::string in2, std::string in3, std::string in4, int turn);


    // Generates a CSV file for game tree data (used for analytics or AI training)
    // Parameters:
    // - out: Output file stream for player 1's data
    // - out2: Output file stream for player 2's data
    // - in1, in2, in3, in4, in5: Paths to input CSV files for deck, piles, hands, moves, and hand knowledge
    // - turn: Current turn number
    // Returns: 0 on success
    int tree_csv(std::ofstream &out, std::ofstream &out2, std::string in1, std::string in2, std::string in3, std::string in4, 
        std::string in5, int turn);

    // Retrieves the current state of the game
    // Returns: The current game state
    State get_curr_state();

    // Runs a test version of the game loop with pre-selected moves
    // Parameters:
    // - log_game: If true, logs the game details to the console
    // - test_moves: A vector of pre-selected moves to simulate the game
    // Returns: Final game score
    int run_test(bool log_game, std::vector<move> test_moves);
};

