#include "openings-book.h"
#include "mancala-engine.h"
#include "timer.h"
#include <fstream>

OpeningsBookGenerator::OpeningsBookGenerator()
{
}



void OpeningsBookGenerator::Generate()
{
	State* state = new State();
    state->MutateBoard({4,4,4,1,1,1,5,2,7,5,2,6,5,1});
    state->ChangeRuleset(1);
    state->ChangeTurn(1);
    Recursive(state, {5, 10, 7, 5, 3, 5, 4}, 9);
}

void OpeningsBookGenerator::Recursive(State* state, const std::vector<char>& history, int size)
{
    if (state->m_Turn == 0)
    {
        std::vector<char> _history = history;
        Minimax* engine = new Minimax();
        float duration = 0.0; // Initialize duration for time limit
        char depth = 8; // Initial depth for minimax search
        char bestMove = -1; // Initialize best move
        int timeLimit = 100;
        // Perform iterative deepening search until time limit is reached
        while (duration < timeLimit && depth < 80)
        {
            Timer timer(&duration); // Start timer
            engine->BestMove(*state, depth); // Get best move using minimax with current depth
            depth++; // Increment depth for next iteration
        }

        bestMove = engine->BestMove(*state, depth);
        _history.push_back(bestMove);
        auto nextState = state->NextState(bestMove);
        Recursive(&nextState, _history, size);
    }
    else
    {
        if (history.size()>size)
        {
            std::fstream file;
            file.open("db/book.txt", std::ios::app);
            for (int i = 0; i < history.size(); ++i)
            {
                if (i != history.size() - 1)
                {
                    file << (int)history[i] << " ";
                }
                else
                {
                    file << (int)history[i];
                }
            }
            file << "\n";
        }
        else {
            auto legalMoves = state->LegalMoves();
            for (size_t i = 0; i < legalMoves.size(); i++)
            {
                std::vector<char> _history = history;
                _history.push_back(legalMoves[i]);
                auto nextState = state->NextState(legalMoves[i]);
                Recursive(&nextState, _history, size);
            }
        }
    }
}

