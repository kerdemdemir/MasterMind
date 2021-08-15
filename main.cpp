#define DOCTEST_CONFIG_IMPLEMENT

#include "Game.h"
#include "UnitTests/UnitTests.h"

#include <iostream>

//! Helper Function for Program Option
/*!
    If option exists returns true. Forexample if third param options is -b and user inputs -b2, func will
    return true
*/
bool cmdOptionExists(char** begin, char** end, const std::string& option)
{
    return std::find(begin, end, option) != end;
}

//! main function
/*!
    I decided to keep Unit test and application within same program. I used "doctest" for unit test framework.
    If user uses "-t" as option than unit tests will trigger.
*/
int main( int argc, char *argv[] )
{
    if (cmdOptionExists(argv, argv + argc, "-t"))
    {
        doctest::Context context;
        int res = context.run();
        std::cout << res;
        return 0;
    }
    else
    {
        std::cout << "Codebreaker please select your strategy by pressing: " << std::endl
                  << "(1)-Play as Human" << std::endl
                  << "(2)-Minimax algorithm developed by Donald Knuth will be play by computer " << std::endl
                  << "(3)-Swaszek algorithm which requires no bookkeeping and still wins" << std::endl;

        int userInput;
        std::cin >> userInput;
        if ( userInput < 1 || userInput > 3 )
        {
            std::cout << "Invalid input program will be terminated" << std::endl;
        }

        Common::GameMode gameMode = static_cast<Common::GameMode>(userInput-1);
        Game game(gameMode);
        game.StartTheGame();
    }

    return 0;
}

