#include "game.h"

// if you are on windows change "clear" to "cls"
#define CLEAR_COMMAND "clear"


Game::Game()
{
    m_Player1 = MINIMAX;
    m_Player2 = MINIMAX; 
    m_State = new State();

    cnf_RULESET = 0; // Classical ruleset
    cnf_TIME_LIMIT = 100; // 100ms
    cnf_OPENING_MOVE_ALLOWED = 0; // Allow

    ReadSettings();
}

Game::~Game()
{
    // Add any necessary cleanup code here
}

void Game::ReadSettings()
{

    
    // Ruleset settings [Classical, Turkish]
    {
        std::ifstream configFile;
        configFile.open("./settings/default_ruleset.dat", std::ios::binary);

        if (!configFile)
        {
            std::ofstream _configFile;
            std::filesystem::create_directory("settings");
            _configFile.open("./settings/default_ruleset.dat", std::ios::binary);
            _configFile.write((char*)&cnf_RULESET, sizeof(cnf_RULESET));
            _configFile.close();
        }
        else
        {
            configFile.read((char*)&cnf_RULESET, sizeof(cnf_RULESET));
            configFile.close();
        }
    }

    // Minimax algorithm timeout: default 100ms
    {
        std::ifstream configFile;
        configFile.open("./settings/minimax_time_limit.dat", std::ios::binary);

        if (!configFile)
        {
            std::ofstream _configFile;
            _configFile.open("./settings/minimax_time_limit.dat", std::ios::binary);
            _configFile.write((char*)&cnf_TIME_LIMIT, sizeof(cnf_TIME_LIMIT));
            _configFile.close();
        }
        else
        {
            configFile.read((char*)&cnf_TIME_LIMIT, sizeof(cnf_TIME_LIMIT));
            configFile.close();
        }
    }

    // There is particular opening move in classical mancala that gives unfair advantage to the player. User can block this opening.
    {
        std::ifstream configFile;
        configFile.open("./settings/opening_move_permission.dat", std::ios::binary);

        if (!configFile)
        {
            std::ofstream _configFile;
            _configFile.open("./settings/opening_move_permission.dat", std::ios::binary);
            _configFile.write((char*)&cnf_OPENING_MOVE_ALLOWED, sizeof(cnf_OPENING_MOVE_ALLOWED));
            _configFile.close();
        }
        else
        {
            configFile.read((char*)&cnf_OPENING_MOVE_ALLOWED, sizeof(cnf_OPENING_MOVE_ALLOWED));
            configFile.close();
        }
    }

    Menu();
}

void Game::Menu()
{

    system(CLEAR_COMMAND);

    std::cout << "0. Play\n1. Settings\n\n";
    std::cout << "> ";

    int option;
    std::cin >> option;

    // Process user input
    switch (option)
    {
    case 0: // Play
    {
        Initialize();
    }
    break;
    case 1: // Settings
    {
        Settings();
    }
    break;
    default: // Invalid Option
    {
        Menu(); // Display the menu again
    }
    break;
    }
}

void Game::Initialize()
{

    system(CLEAR_COMMAND);

    // Set up player 1
    {
        std::cout << "0. Minimax\n1. Player\n\n";
        std::cout << "Select agent for Player1\n> ";

        int type;
        std::cin >> type;

        m_Player1 = AgentEnum(type);

        system(CLEAR_COMMAND);
    }

    // Set up player 2
    {
        std::cout << "0. Minimax\n1. Player\n\n";
        std::cout << "Select agent for Player2\n> ";

        int type;
        std::cin >> type;

        m_Player2 = AgentEnum(type);

        system(CLEAR_COMMAND);
    }

    // Determine whose turn it is
    {
        std::cout << "0. Player1\n1. Player2\n\n";
        std::cout << "Whose turn\n> ";

        int turn;
        std::cin >> turn;

        m_State = new State();
        m_State->ChangeTurn((char)turn);

        system(CLEAR_COMMAND); 
    }

    m_State->m_Ruleset = cnf_RULESET;
    Play(); // Start the game loop
}

void Game::Settings()
{

    system(CLEAR_COMMAND);

    std::cout << "0. Change ruleset\n1. Edit time limit for minimax algorithm\n2. Change 2-5 opening move permission\n3. Exit\n\n> ";
    int option = -1;
    std::cin >> option;

    switch (option)
    {
    case 0: {
        system(CLEAR_COMMAND);
        std::cout << "0. Classical Mancala Ruleset\n1. Turkish Mancala Ruleset\n\n> ";
        int _option;
        std::cin >> _option;

        switch (_option)
        {
        case 0: {
            cnf_RULESET = 0;
            {
                std::ofstream configFile;
                configFile.open("./settings/default_ruleset.dat", std::ios::binary);
                configFile.write((char*)&cnf_RULESET, sizeof(cnf_RULESET));
                configFile.close();
            }
            Settings();
        }; break;
        case 1: {
            cnf_RULESET = 1;
            {
                std::ofstream configFile;
                configFile.open("./settings/default_ruleset.dat", std::ios::binary);
                configFile.write((char*)&cnf_RULESET, sizeof(cnf_RULESET));
                configFile.close();
            }
            Settings();
        }; break;
        default: {
            Settings();
        }; break;
        }

    }; break;
    case 1: {
        system(CLEAR_COMMAND);
        std::cout << "Enter the time limit in miliseconds\n\n> ";
        int _value;
        std::cin >> _value;

        cnf_TIME_LIMIT = _value;
        {
            std::ofstream configFile;
            configFile.open("./settings/minimax_time_limit.dat", std::ios::binary);
            configFile.write((char*)&cnf_TIME_LIMIT, sizeof(cnf_TIME_LIMIT));
            configFile.close();
        }

        Settings();
    }; break;
    case 2: {
        system(CLEAR_COMMAND);
        std::cout << "0. Allow\n1. Don't Allow\n\n> ";
        int _value;
        std::cin >> _value;

        cnf_OPENING_MOVE_ALLOWED = _value == 0 ? 1 : 0;
        {
            std::ofstream configFile;
            configFile.open("./settings/opening_move_permission.dat", std::ios::binary);
            configFile.write((char*)&cnf_OPENING_MOVE_ALLOWED, sizeof(cnf_OPENING_MOVE_ALLOWED));
            configFile.close();
        }

        Settings();

    }; break;
    case 3: {
        Menu();
    }; break;
    default: {
        Settings();
    };break;
    }
}

void Game::Play()
{
    while (m_State->GameState() != GAMEOVER)
    {
        if (m_State->m_Turn == 0)
        {
            if (m_Player1 == PLAYER)
            {
                GetPlayerMove();
            }
            else
            {
                GetAIMove(m_Player1);
            }
        }
        else
        {
            if (m_Player2 == PLAYER)
            {
                GetPlayerMove();
            }
            else
            {
                GetAIMove(m_Player2);
            }
        }
    }

    SaveGame(); // Save the game state when the game is over
}

void Game::GetPlayerMove()
{
    std::cout << "[PLAYER] Player" << int(m_State->m_Turn + 1) << ": ";
    int move;
    std::cin >> move; // Get move input from the player

    if (m_State->IsLegal(move))
    {
        m_State->MakeMove(move); // Make the move if it is legal
        history.push_back(move); // Record the move in the history
        m_State->Print(); // Print the updated game state
    }
    else
    {
        std::cerr << "Invalid Move: " << move << std::endl; // Print error message for invalid move
    }
}

void Game::GetAIMove(AgentEnum agent)
{
    if (agent == MINIMAX)
    {
        Minimax engine;
        std::cout << "[AI] Player" << int(m_State->m_Turn + 1) << ": ";

        float duration = 0.0; // Initialize duration for time limit
        char depth = 5; // Initial depth for minimax search
        char bestMove = -1; // Initialize best move

        // Perform iterative deepening search until time limit is reached
        while (duration < cnf_TIME_LIMIT && depth < 80)
        {
            Timer timer(&duration); // Start timer
            bestMove = engine.BestMove(*m_State, depth); // Get best move using minimax with current depth
            depth++; // Increment depth for next iteration
        }

        // Get best move using final depth
        bestMove = engine.BestMove(*m_State, depth);

        // Make the best move in the game
        if (!cnf_OPENING_MOVE_ALLOWED && (history.size() == 1 && history.at(0) == 2))
        {
            // Special case handling
            std::cout << 4 << std::endl;
            std::cout << "depth: " << (int)depth << std::endl;

            m_State->MakeMove(4); // Make the move
            history.push_back(4); // Record the move in the history
        }
        else
        {
            // Normal move handling
            std::cout << (int)bestMove << std::endl;
            std::cout << "depth: " << (int)depth << std::endl;

            m_State->MakeMove(bestMove); // Make the move
            history.push_back(bestMove); // Record the move in the history
        }

        float score = engine.EvaluationScore(*m_State);

        std::cout << "evaluation score: " << score << std::endl;

        m_State->Print(); // Print the updated game state
    }
}

void Game::SaveGame()
{
    const char BEGINNING_OF_GAME = 0xFE; // beginning-of-game flag
    const char END_OF_GAME = 0xFF; // end-of-game flag
    int gamesCount = 0; // number of games played

    // Read the number of games played from file or initialize to zero
    {
        std::ifstream countFile;
        countFile.open("./db/games/count.dat", std::ios::binary);

        if (!countFile)
        {
            std::ofstream _countFile;
            std::filesystem::create_directory("db");
            _countFile.open("./db/games/count.dat", std::ios::binary);
            _countFile.write((char*)&gamesCount, sizeof(gamesCount));
            _countFile.close();
        }
        else
        {
            countFile.read((char*)&gamesCount, sizeof(gamesCount));
            countFile.close();
        }
    }

    // Generate file name based on game count
    std::string fileName = "./db/games/" + GetFileNameWithLeadingZeros(gamesCount) + ".dat";

    // Write game history to file
    {
        std::ofstream file(fileName, std::ios::binary | std::ios::app);
        file.put((char)m_State->m_Ruleset);
        file.put(BEGINNING_OF_GAME); 
        for (char move : history)
        {
            file.put(move); // Write each move to file
        };
        file.put(END_OF_GAME);
        file.close(); 
    }

    std::cout << "Game saved successfully!\n";

    // Update games count and write it back to file
    {
        gamesCount+=1; 
        std::ofstream countFile;
        countFile.open("./db/games/count.dat", std::ios::binary);
        countFile.write((char*)&gamesCount, sizeof(gamesCount));
        countFile.close();
    }
}

std::string Game::GetFileNameWithLeadingZeros(int fileName)
{
    std::string fileNameStr = ""; 
    int fileNameLength = (int)std::to_string(fileName).size(); // Get length of original file name
    for (int i = 0; i < 8 - fileNameLength; i++) // Add leading zeros to make total length 8 characters
    {
        fileNameStr += "0";
    }
    fileNameStr += std::to_string(fileName); // Append original file name
    return fileNameStr; // Return file name with leading zeros
}
