#include "state-analyzer.h"
#include "timer.h"
#include <iostream>
#include <string>
#include <stdio.h>

// if you are on windows change "clear" to "cls"
#define CLEAR_COMMAND "clear"

void StateAnalyzer::AnalyzeState(State*& state, const int& timeLimit)
{
	auto engine = new Minimax();
	std::cout << "\nanalayzing state...\n";
    float duration = 0.0; // Initialize duration for time limit
    char depth = 8; // Initial depth for minimax search
    char bestMove = -1; // Initialize best move

    // Perform iterative deepening search until time limit is reached
    while (duration < timeLimit && depth < 80)
    {
        Timer timer(&duration); // Start timer
        engine->BestMove(*state, depth, true); // Get best move using minimax with current depth
        depth++; // Increment depth for next iteration
    }

    // Get best move using final depth
    std::cout << "depth: " << (int)depth << "\n";
    engine->BestMove(*state, depth, true);
}

void StateAnalyzer::Start(const char& ruleset)
{
    while (true)
    {
        system(CLEAR_COMMAND);
        State* state = new State();
        state->ChangeRuleset(ruleset);
        std::cout << "Enter the board state to analyze\n\nboard > ";
        std::string stateString;
        std::cin >> stateString;

        std::vector<char> board = {};
        std::string buffer = "";
        for (int i = 0; i < stateString.size(); i++)
        {
            if (stateString[i] == '-')
            {
                board.push_back((char)std::stoi(buffer));
                buffer = "";
            }
            else {
                buffer = buffer + stateString[i];
            }
        }
        board.push_back((char)std::stoi(buffer));

        if (board.size() != 14)
        {
            std::cout << "Wrong board representation!\n";
        }
        else
        {
            std::string turn;
            std::cout << "\nturn > ";
            std::cin >> turn;

            state->ChangeTurn((char)std::stoi(turn));
            state->MutateBoard(board);

            AnalyzeState(state, 1000);

            system("PAUSE");
        }
    }
}
