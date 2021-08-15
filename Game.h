#pragma once

#include "CodeBreaker.h"
#include "CodeMaker.h"

//! Game mediates between CodeBreaker and CodeKeeper
/*!
    Game has two responsibilities first it runs the game by mediating between CodeBreaker and CodeKeeper.
    Second it helps CodeBreaker's initilization by setting it strategy and feeding all possible inputs.
*/
class Game
{
public:
    Game( Common::GameMode mode ) : gameMode(mode)
    {
        codeBreaker.SetAllCodes(GenerateAllPossibleCodes());

        if ( mode == Common::GameMode::Human )
        {
            codeBreaker.SetStrategy(std::make_shared<HumanStrategy>());
        }
        else if ( mode == Common::GameMode::MiniMax)
        {
            codeBreaker.SetStrategy(std::make_shared<MiniMaxStrategy>());
        }
        else
        {
            codeBreaker.SetStrategy(std::make_shared<SwaszekStrategy>());
        }
    }

    int StartTheGame()
    {
        if ( gameMode != Common::GameMode::Human )
            std::cout << "Human observer secret is: " << codeMaker.GetSecretCode().ToString() << std::endl;
        for ( int i = 0; i < MaximumRoundCount; i++ )
        {
            auto guess = codeBreaker.Guess();
            std::cout << "Code breaker's " << i << ". guess was "  << guess.ToString() << std::endl;
            auto result = codeMaker.GetResultOfGuess(guess);
            std::cout << "Code maker's judgement: " <<  result.ToString() << std::endl;
            codeBreaker.SetResult(result);
            if ( result.blackCount == LengthOfSecret )
            {
                if ( gameMode == Common::GameMode::Human )
                    std::cout << "You won!! It took you : " << i << " rounds " << std::endl;
                else
                    std::cout << "It took computer to won in : " << i << " rounds " << std::endl;
                return i;
            }
        }
        std::cout << "Code was not cracked! Codebreaker loses codekeeper wins" << std::endl;
        return -1;
    }

    std::vector<Common::Code> GenerateAllPossibleCodes()
    {
        std::vector<Common::Code> returnVal;
        int startingInt = Common::GetStartingInteger();
        Common::Code code(startingInt);
        returnVal.push_back(code);
        while ( true )
        {
            auto newCode = Common::Code(code.NextCode());
            if ( newCode == code)
                break;
            returnVal.push_back(newCode);
            code = newCode;
        }
        return returnVal;
    }

private:
    Common::GameMode gameMode;
    CodeBreaker codeBreaker;
    CodeMaker   codeMaker;
};