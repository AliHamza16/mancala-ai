#pragma once

#include <ctime>
#include <format>
#include <iostream>
#include <vector>
#include <string>

/**
 * @brief Enumerates the possible states of the game.
 *
 * The GameStateEnum enum represents the possible states of the game, including playing and game over.
 */
enum GameStateEnum
{
	PLAYING,
	GAMEOVER
};

/**
 * @brief Represents the state of the game.
 *
 * The State class encapsulates the state of the game board and provides methods for interacting
 * with the board and playing the game.
 */
class State
{
	friend class Minimax;
	friend class Game;
	friend class OpeningsBookGenerator;

private:
	std::vector<char> m_Board;
	char m_Ruleset;
	char m_Turn;

	void InitializeState();
	void ClearPits(const char &pit);
	void ClearPits(const char &start, const char &stop);
	void MakeMove(const char &move);
	void ClassicalMancalaRuleset(const char &move);
	void TurkishMancalaRuleset(const char &move);

	std::vector<char> LegalMoves() const;
	std::string GetStateString(int depth) const;
	State NextState(const char &move) const;
	char TotalStones(const char &start, const char &stop) const;
	char OppositePit(const char &pit);
	char RandomMove();
	char GetWinner() const;

	bool IsLegal(const char &move);

	GameStateEnum GameState() const;

public:
	State();
	~State();
	void MutateBoard(const std::vector<char> &board);
	void Print();
	void ChangeTurn(const char &turn);
	void ChangeRuleset(const char &ruleset);
};