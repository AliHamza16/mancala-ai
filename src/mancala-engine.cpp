#include "mancala-engine.h"
#include "timer.h"


/**
 * @brief Constructs a Minimax object.
 *
 * This constructor initializes a Minimax object.
 */
Minimax::Minimax()
{
    m_Ruleset = 0;
}

/**
 * @brief Destructs the Minimax object.
 *
 * This destructor cleans up resources associated with the Minimax object.
 */
Minimax::~Minimax()
{
}

/**
 * @brief Applies the Minimax algorithm with alpha-beta pruning to determine the optimal move.
 *
 * This function recursively applies the Minimax algorithm with alpha-beta pruning to search for the optimal move.
 *
 * @param state The current game state.
 * @param depth The maximum depth to search in the game tree.
 * @param alpha The alpha value for alpha-beta pruning.
 * @param beta The beta value for alpha-beta pruning.
 * @param maximizingPlayer Indicates whether the current player is maximizing (true) or minimizing (false).
 * @return The minimax value of the current state.
 */
float Minimax::minimax(const State& state, const char& depth, const float& alpha, const float& beta, const char& maximizingPlayer)
{
    if (depth == 0 || state.GameState() == GAMEOVER)
    {
        return Evaluate(state);
    }

    if (maximizingPlayer)
    {
        float value = -9999.0F, _alpha = alpha, _beta = beta;
        std::vector<char> legalMoves = state.LegalMoves();
        for (const char& move : legalMoves)
        {
            const State& nextState = state.NextState(move);
            value = std::max(value, minimax(nextState, depth - 1, _alpha, _beta, nextState.m_Turn == 0));
            if (value >= _beta)
            {
                break;
            }
            _alpha = std::max(_alpha, value);
        }
        return value;
    }
    else
    {
        float value = 9999.0F, _alpha = alpha, _beta = beta;
        std::vector<char> legalMoves = state.LegalMoves();
        for (const char& move : legalMoves)
        {
            const State& nextState = state.NextState(move);
            value = std::min(value, minimax(nextState, depth - 1, _alpha, _beta, nextState.m_Turn == 0));
            if (value <= _alpha)
            {
                break;
            }
            _beta = std::min(_beta, value);
        }
        return value;
    }
}

/**
 * @brief Evaluates the current game state.
 *
 * This function evaluates the current game state to determine its desirability for the maximizing player.
 *
 * @param state The current game state.
 * @return The evaluation score of the state.
 */
float Minimax::Evaluate(const State& state)
{
    if (state.m_Board[6] > 24)
    {
        return 9999;
    }
    else if (state.m_Board[13] > 24)
    {
        return -9999;
    }
    else {
        int ourStoreIndex = 6 + state.m_Turn * 7;
        int opponentsStoreIndex = 6 + (1-state.m_Turn) * 7;
        int ourCaptureOpportunities = 0;
        int opponentsCaptureOpportunities = 0;

        State* copyState = new State();
        copyState->m_Board = state.m_Board;
        copyState->m_Ruleset = state.m_Ruleset;
        copyState->m_Turn = state.m_Turn;

        std::vector<char> legalMoves = copyState->LegalMoves();
        std::vector<State*> nextStates = {};
        for (char move : legalMoves)
        {
            auto nextState = copyState->NextState(move);
            nextStates.push_back(&nextState);
        }
        for (const State* _state : nextStates)
        {
            ourCaptureOpportunities += _state->m_Board[ourStoreIndex] - copyState->m_Board[ourStoreIndex];
        }

        copyState->m_Turn = 1 - state.m_Turn;
        legalMoves = copyState->LegalMoves();
        nextStates = {};
        for (char move : legalMoves)
        {
            auto nextState = copyState->NextState(move);
            nextStates.push_back(&nextState);
        }
        for (const State* _state : nextStates)
        {
            opponentsCaptureOpportunities += _state->m_Board[opponentsStoreIndex] - copyState->m_Board[opponentsStoreIndex];
        }

        return (1 - 2 * state.m_Turn) * 0.8 * (state.m_Board[ourStoreIndex] - state.m_Board[opponentsStoreIndex]) + (1 - 2 * state.m_Turn)* 0.2 * (ourCaptureOpportunities - opponentsCaptureOpportunities);
    }
 }

/**
 * @brief Calculates the best move using the Minimax algorithm.
 *
 * This function computes the best move for the current player using the Minimax algorithm with alpha-beta pruning.
 *
 * @param state The current game state.
 * @param depth The maximum depth to search in the game tree.
 * @return The best move to make.
 */
char Minimax::BestMove(const State& state, const char& depth, const bool &log)
{
    std::vector<char> legalMoves = state.LegalMoves();

    if (state.m_Turn == 0)
    {
        float bestValue = -9999.0F;
        char bestMove = -1;

        for (const char& move : legalMoves)
        {
            const State& nextState = state.NextState(move);
            float val = minimax(nextState, depth, -9999.0F, 9999.0F, nextState.m_Turn == 0);

            if (log)
            {
                std::cout << "move: " << (int)move << " score: " << val << "\n";
            }

            if (val >= bestValue)
            {
                bestValue = val;
                bestMove = move;
            }
        }

        if (bestMove == -1)
        {
            std::cout << "Error\n";
        }

        return bestMove;
    }
    else
    {
        float bestValue = 9999.0F;
        char bestMove = -1;
        for (const char& move : legalMoves)
        {
            const State& nextState = state.NextState(move);
            float val = minimax(nextState, depth, -9999.0F, 9999.0F, nextState.m_Turn == 0);

            if (log)
            {
                std::cout << "move: " << (int)move << " score: " << val << "\n";
            }

            if (val <= bestValue)
            {
                bestValue = val;
                bestMove = move;
            }
        }

        if (bestMove == -1)
        {
            std::cout << "Error\n";
        }

        return bestMove;
    }
}

float Minimax::EvaluationScore(const State& state)
{

    float duration = 0.0; // Initialize duration for time limit
    char depth = 5; // Initial depth for minimax search
    float score = 0;

    while (duration < 10 && depth < 80)
    {
        Timer timer(&duration); // Start timer
        score = minimax(state, depth, -9999.0F, 9999.0F, state.m_Turn == 0);
        depth++; // Increment depth for next iteration
    }

    return score;
}
