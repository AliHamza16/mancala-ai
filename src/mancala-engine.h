#pragma once

#include <iostream>
#include <format>
#include <string>
#include <vector>
#include <algorithm>
#include <ctime>

#include "state.h"



/**
 * @brief A class for implementing the Minimax algorithm.
 *
 * The Minimax class provides functionality for applying the Minimax algorithm to determine the best move
 * in a game.
 */
class Minimax
{
private:

	int m_Ruleset;
	float minimax(const State& state, const char& depth, const float& alpha, const float& beta, const char& maximizing_player);
	float Evaluate(const State& state);


public:
	Minimax();
	~Minimax();


	char BestMove(const State& state, const char& depth, const bool& log = false);
	float EvaluationScore(const State& state);
};
