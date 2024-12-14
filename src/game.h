#pragma once

#include <filesystem>
#include <fstream>
#include <string>
#include <stdio.h>

#include "mancala-engine.h"
#include "state.h"
#include "timer.h"


enum AgentEnum
{
	MINIMAX,
	PLAYER  
};


class Game
{
public:
    Game();

    ~Game();

private:
    State* m_State; // Game state
    AgentEnum m_Player1; 
    AgentEnum m_Player2;

    int cnf_TIME_LIMIT; // default 100ms
    int cnf_OPENING_MOVE_ALLOWED;
    int cnf_RULESET;

    void ReadSettings();

    void Menu();

    void Initialize();

    void Settings();

    void Play();

    void GetPlayerMove();

    void GetAIMove(AgentEnum agent);

    void SaveGame();

    std::string GetFileNameWithLeadingZeros(int fileName);

    std::vector<char> history; // history of played moves
};
