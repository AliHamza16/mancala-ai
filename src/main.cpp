#include <memory>
#include "game.h"
#include "openings-book.h"
#include "state-analyzer.h"

int main()
{
    std::unique_ptr<Game> game = std::make_unique<Game>();
    /*std::unique_ptr<OpeningsBookGenerator> book = std::make_unique<OpeningsBookGenerator>();
    book->Generate();*/
    /*StateAnalyzer::Start();*/
    return 0;
}