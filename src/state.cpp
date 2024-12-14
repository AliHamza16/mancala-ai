#include "state.h"

/**
 * @brief Constructor for the State class.
 *
 * Initializes the game state.
 */
State::State()
{
    InitializeState(); // Call the function to initialize the state
};

/**
 * @brief Destructor for the State class.
 *
 * Cleans up resources used by the game state.
 */
State::~State()
{
}

void State::MutateBoard(const std::vector<char>& board)
{
    for (size_t i = 0; i < 14; ++i)
    {
        // Initialize each slot in the board
        m_Board[i] = board[i];
    }
}



/**
 * @brief Initializes the game state.
 */
void State::InitializeState()
{
    m_Board.resize(14); // Resize the board vector to 14 elements
    srand(time(0)); // Seed the random number generator
    for (size_t i = 0; i < 14; ++i)
    {
        // Initialize each slot in the board
        m_Board[i] = (i == 6 || i == 13) ? 0 : 4; // Check if the slot is a store or a regular slot
    }
    m_Turn = 0; // Set the initial turn to player 1
    m_Ruleset = 0;
}


/**
 * @brief Prints the current game state to the console.
 */
void State::Print()
{
    // Print the second player's slots and store
    std::cout << std::format("({0}{1})", m_Board[13] < 10 ? " " : "", (int)m_Board[13]);
    for (size_t i = 12; i > 6; --i)
    {
        std::cout << std::format(" [{0}{1}]", m_Board[i] < 10 ? " " : "", (int)m_Board[i]);
    };
    std::cout << std::endl;

    // Print the first player's slots and store
    std::cout << "    ";
    for (size_t i = 0; i < 6; ++i)
    {
        std::cout << std::format(" [{0}{1}]", m_Board[i] < 10 ? " " : "", (int)m_Board[i]);
    };
    std::cout << std::format(" ({0}{1})", m_Board[6] < 10 ? " " : "", (int)m_Board[6]) << std::endl
        << std::endl;
}


/**
 * @brief Clears the stones from a specified pit.
 *
 * @param pit The index of the pit to clear.
 */
void State::ClearPits(const char& pit)
{
    m_Board[pit] = 0; // Set the stones in the specified pit to zero
};


/**
 * @brief Clears the stones from pits within a specified range.
 *
 * @param start The index of the starting pit (inclusive).
 * @param stop The index of the ending pit (exclusive).
 */
void State::ClearPits(const char& start, const char& stop)
{
    for (int i = start; i < stop; ++i)
    {
        m_Board[i] = 0; // Set the stones in each pit within the range to zero
    }
};


/**
 * @brief Calculates the total number of stones in pits within a specified range.
 *
 * @param start The index of the starting pit (inclusive).
 * @param stop The index of the ending pit (exclusive).
 * @return The total number of stones in the specified range of pits.
 */
char State::TotalStones(const char& start, const char& stop) const
{
    char total = 0; // Initialize total stones counter

    // Iterate through pits within the specified range and sum up the number of stones
    for (size_t i = start; i < stop; ++i)
    {
        total += m_Board[i]; // Add stones from each pit to the total
    }
    return total; // Return the total number of stones
};


/**
 * @brief Finds the index of the pit opposite to the specified pit.
 *
 * @param pit The index of the pit.
 * @return The index of the pit opposite to the specified pit.
 */
char State::OppositePit(const char& pit)
{
    return (12 - pit); // Calculate the index of the opposite pit
};


/**
 * @brief Finds the legal moves for the current player.
 *
 * @return A vector containing the indices of legal moves.
 */
std::vector<char> State::LegalMoves() const
{
    std::vector<char> legalMoves; // Initialize vector to store legal moves

    // Determine the range of pits to consider based on the current player's turn
    size_t start = m_Turn == 0 ? 0 : 7; // Start index for player 1 or player 2
    size_t stop = m_Turn == 0 ? 6 : 13; // Stop index for player 1 or player 2

    // Iterate through the pits within the range and check if they contain stones
    for (size_t i = start; i < stop; ++i)
    {
        if (m_Board[i] > 0)
        {
            legalMoves.push_back((char)i); // Add index of pit with stones to legal moves
        }
    }

    return legalMoves; // Return vector of legal moves
}


/**
 * @brief Checks if a move is legal.
 *
 * @param move The index of the pit to move stones from.
 * @return True if the move is legal, false otherwise.
 */
bool State::IsLegal(const char& move)
{
    for (auto& m : LegalMoves()) // Iterate through legal moves
    {
        if (m == move) // Check if the specified move is in the list of legal moves
        {
            return true; // Return true if the move is legal
        }
    }
    return false; // Return false if the move is not legal
}


/**
 * @brief Generates a random legal move.
 *
 * This function randomly selects a legal move from the available legal moves and returns it.
 *
 * @return A randomly selected legal move.
 */
char State::RandomMove()
{
    const std::vector<char> legalMoves = LegalMoves(); // Get the list of legal moves
    const char move = legalMoves.at(rand() % legalMoves.size()); // Choose a random move from the legal moves
    return move; // Return the randomly selected move
}


/**
 * @brief Makes a move in the game.
 *
 * This function applies the rules of the game to make a move.
 *
 * @param move The index of the pit to move stones from.
 */
void State::MakeMove(const char& move)
{
    switch (m_Ruleset)
    {
    case 0: {
        ClassicalMancalaRuleset(move);// Apply the classical Mancala ruleset to make the move
    }; break;
    case 1: {
        TurkishMancalaRuleset(move);// Apply the Turkish Mancala ruleset to make the move
    }; break;
    default:
        break;
    }
}


/**
 * @brief Applies the classical Mancala ruleset to make a move.
 *
 * This function implements the classical Mancala ruleset to make a move in the game.
 *
 * @param move The index of the pit to move stones from.
 */
void State::ClassicalMancalaRuleset(const char& move)
{
    char stoneCount = m_Board[move]; // Get the number of stones in the selected pit
    ClearPits(move); // Clear the selected pit

    // Define variables for pits and stores based on current player's turn
    const char ourStore = m_Turn == 0 ? 6 : 13; // Index of our store
    const char ourStart = m_Turn == 0 ? 0 : 7; // Index of our starting pit
    const char ourStop = m_Turn == 0 ? 6 : 13; // Index of our stopping pit
    const char oppStore = m_Turn == 0 ? 13 : 6; // Index of opponent's store
    const char oppStart = m_Turn == 0 ? 7 : 0; // Index of opponent's starting pit
    const char oppStop = m_Turn == 0 ? 13 : 6; // Index of opponent's stopping pit

    char currentPit = (move + 1) % 14; // Index of the next pit to distribute stones to
    char lastPit = currentPit; // Initialize the last pit visited

    // Distribute stones to pits according to Mancala rules
    while (stoneCount > 0)
    {
        ++m_Board[currentPit]; // Place a stone in the current pit
        --stoneCount; // Decrement the remaining stones

        lastPit = currentPit; // Update the last pit visited

        if (currentPit == oppStore) // If the last stone lands in the opponent's store
        {
            --m_Board[currentPit]; // Remove the stone from the opponent's store
            ++stoneCount; // Increment the remaining stones
        }

        currentPit = (currentPit + 1) % 14; // Move to the next pit
    }

    // Check and apply game rules [1]
    {
        m_Turn = ((m_Turn == 0 && lastPit == 6) || (m_Turn == 1 && lastPit == 13)) ? m_Turn : (1 - m_Turn);
    }

    // Check and apply game rules [2]
    {
        if (m_Board[lastPit] == 1 && (lastPit >= ourStart && lastPit < ourStop) && m_Board[OppositePit(lastPit)] != 0)
        {
            // If the last stone lands in an empty pit on our side and the opposite pit is not empty
            m_Board[ourStore] += m_Board[OppositePit(lastPit)] + 1; // Move stones to our store
            ClearPits(lastPit); // Clear the last pit
            ClearPits(OppositePit(lastPit)); // Clear the opposite pit
        }
    }

    // Check and apply game rules [3]
    {
        const char ourTotal = TotalStones(ourStart, ourStop); // Total stones on our side
        const char oppTotal = TotalStones(oppStart, oppStop); // Total stones on opponent's side

        if (ourTotal == 0) // If we have no stones left on our side
        {
            m_Board[oppStore] += oppTotal; // Move opponent's stones to their store
            ClearPits(oppStart, oppStop); // Clear opponent's pits
        }
        else if (oppTotal == 0) // If opponent has no stones left on their side
        {
            m_Board[ourStore] += ourTotal; // Move our stones to our store
            ClearPits(ourStart, ourStop); // Clear our pits
        }
    }
}


/**
 * @brief Applies the Turkish Mancala ruleset to make a move.
 *
 * This function implements the Turkish Mancala ruleset to make a move in the game.
 *
 * @param move The index of the pit to move stones from.
 */
void State::TurkishMancalaRuleset(const char& move)
{
    char stoneCount = m_Board[move]; // Get the number of stones in the selected pit
    ClearPits(move); // Clear the selected pit

    // Define variables for pits and stores based on current player's turn
    const char ourStore = m_Turn == 0 ? 6 : 13; // Index of our store
    const char ourStart = m_Turn == 0 ? 0 : 7; // Index of our starting pit
    const char ourStop = m_Turn == 0 ? 6 : 13; // Index of our stopping pit
    const char oppStore = m_Turn == 0 ? 13 : 6; // Index of opponent's store
    const char oppStart = m_Turn == 0 ? 7 : 0; // Index of opponent's starting pit
    const char oppStop = m_Turn == 0 ? 13 : 6; // Index of opponent's stopping pit

    char currentPit = (move + 1) % 14; // Index of the next pit to distribute stones to
    char lastPit = currentPit; // Initialize the last pit visited

    if (stoneCount > 1)
    {
        m_Board[move]++;
        stoneCount--;
    }

    while (stoneCount > 0)
    {
        ++m_Board[currentPit]; // Place a stone in the current pit
        --stoneCount; // Decrement the remaining stones

        lastPit = currentPit; // Update the last pit visited

        if (currentPit == oppStore) // If the last stone lands in the opponent's store
        {
            --m_Board[currentPit]; // Remove the stone from the opponent's store
            ++stoneCount; // Increment the remaining stones
        }

        currentPit = (currentPit + 1) % 14; // Move to the next pit
    }

    // Check and apply game rules [1]
    {
        m_Turn = ((m_Turn == 0 && lastPit == 6) || (m_Turn == 1 && lastPit == 13)) ? m_Turn : (1 - m_Turn);
    }

    // Check and apply game rules [2]
    {
        if (m_Board[lastPit] == 1 && (lastPit >= ourStart && lastPit < ourStop) && m_Board[OppositePit(lastPit)] != 0)
        {
            // If the last stone lands in an empty pit on our side and the opposite pit is not empty
            m_Board[ourStore] += m_Board[OppositePit(lastPit)] + 1; // Move stones to our store
            ClearPits(lastPit); // Clear the last pit
            ClearPits(OppositePit(lastPit)); // Clear the opposite pit
        }
    }

    // Check and apply game rules [3]
    {
        if (m_Board[lastPit] % 2 == 0 && (lastPit >= oppStart && lastPit < oppStop) && m_Board[lastPit] != 0)
        {
            m_Board[ourStore] += m_Board[lastPit]; 
            ClearPits(lastPit); // Clear the last pit
        }
    }

    // Check and apply game rules [4]
    {
        const char ourTotal = TotalStones(ourStart, ourStop); // Total stones on our side
        const char oppTotal = TotalStones(oppStart, oppStop); // Total stones on opponent's side

        if (ourTotal == 0) // If we have no stones left on our side
        {
            m_Board[ourStore] += oppTotal; // Move opponent's stones to our store
            ClearPits(oppStart, oppStop); // Clear opponent's pits
        }
        else if (oppTotal == 0) // If opponent has no stones left on their side
        {
            m_Board[oppStore] += ourTotal; // Move our stones to opponent's store
            ClearPits(ourStart, ourStop); // Clear our pits
        }
    }
}



/**
 * @brief Changes the current player's turn.
 *
 * @param turn The index of the player whose turn it is (0 or 1).
 */
void State::ChangeTurn(const char& turn)
{
    m_Turn = turn; // Set the current player's turn
}

void State::ChangeRuleset(const char& ruleset)
{
    m_Ruleset = ruleset;
}


/**
 * @brief Generates the next game state after making a move.
 *
 * @param move The index of the pit to move stones from.
 * @return The next game state after making the move.
 */
State State::NextState(const char& move) const
{
    State* state = new State(); // Create a new state object
    state->m_Turn = m_Turn; // Set the turn of the new state
    state->m_Board = m_Board; // Copy the board of the current state
    state->m_Ruleset = m_Ruleset;
    state->MakeMove(move); // Make the specified move in the new state
    return *state; // Return the new state
}


/**
 * @brief Determines the current state of the game.
 *
 * @return The state of the game (PLAYING or GAMEOVER).
 */
GameStateEnum State::GameState() const
{
    if (m_Board[6] + m_Board[13] == 48 || m_Board[6] > 24 || m_Board[13] > 24) // Check if game is over
    {
        return GAMEOVER; // Return GAMEOVER if game conditions are met
    }
    else
    {
        return PLAYING; // Return PLAYING if game is still ongoing
    }
}


/**
 * @brief Determines the winner of the game.
 *
 * @return The index of the winning player (0, 1) or 2 if there is no winner yet.
 */
char State::GetWinner() const
{
    if (m_Board[6] > 24) // If player 1's store has more than 24 stones
    {
        return 0; // Player 1 wins
    }
    else if (m_Board[13] > 24) // If player 2's store has more than 24 stones
    {
        return 1; // Player 2 wins
    }
    else // If neither player has more than 24 stones in their store
    {
        return 2; // No winner yet
    }
}
