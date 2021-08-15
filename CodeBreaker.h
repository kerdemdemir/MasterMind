#pragma once

#include "Common.h"
#include "Strategy.h"

#include <memory>

//! CodeBreaker is responsible by creating a guess code.
/*!
    CodeBreaker is creating guesses. It encapsulates a strategy which can differ itself depending on user command options.
    But independently from strategy there are some algorithms it runs like eliminating codes from possible code list
    if they do not return the same result as its last guess. To be able to feed the strategy it holds a track of
    it guesses, eliminated list of codes and all codes.
*/
class CodeBreaker
{
public:
    CodeBreaker() = default;

    CodeBreaker( std::shared_ptr<IStrategy> strategy ) : strategy(strategy)
    {
    }

    void SetStrategy( std::shared_ptr<IStrategy> strategy )
    {
        this->strategy = strategy;
    }
    void SetAllCodes( const std::vector<Common::Code>& allCodes )
    {
        this->allCodes = allCodes;
        probableCodes = allCodes;
    }

    Common::Code Guess()
    {
        auto returnVal = strategy->Guess(allCodes, probableCodes, pastGuesses);
        pastGuesses.push_back(returnVal);
        return returnVal;
    }

    int SetResult(const Common::Result& currentResult)
    {
        pastResults.push_back(currentResult);
        return Eliminate(currentResult);
    }

private:

    int Eliminate( const Common::Result& currentResult )
    {
        const auto& curGuess = pastGuesses.back();
        std::vector<Common::Code> tempCodes = std::move(probableCodes);
        std::ranges::copy_if( tempCodes, std::back_inserter(probableCodes), [currentResult, curGuess]( const Common::Code& candidateCode ){
            auto tempResult = candidateCode.Compare(curGuess);
            return tempResult == currentResult;
        });

        return probableCodes.size();
    }


    std::vector<Common::Code> allCodes;
    std::shared_ptr<IStrategy> strategy;
    std::vector<Common::Code> probableCodes;
    std::vector<Common::Code> pastGuesses;
    std::vector<Common::Result> pastResults;
};

