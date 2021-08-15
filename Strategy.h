#pragma once

#include "Common.h"

#include <iostream>
#include <unordered_map>

//! IStrategy is algorithm which we use dynamically while guessing
/*!
    We can apply different strategies. For example just for fun we can let a human play the game who will in this case strategy will just get the guess from
    stdin. Or there are more complicated algorithms which uses Entropy like information. Those strategies require a bit more input
    example allCodes, probableCodes(KnuthCodes) etc...
    Making the IStrategy pure virtual makes this program extendable. Another developer can extend with different strategy .
*/
class IStrategy
{
public:
    virtual Common::Code Guess(const std::vector<Common::Code>& allCodes, const std::vector<Common::Code>& probableCode, const std::vector<Common::Code>& pastGuesses) = 0;
};

//! User defined Hash functions for result and code data structures
/*!
    In case of MiniMax algorithm using hashMaps were much more efficient. So I had to come up with user defined hashfunctions.
    I used very simple hashing example for in case of result data structure if there are 2 blacks and 2 whites
    Hash number will be 2 * 10 + 2 = 22. And guaranteed to be unique.
*/
namespace std {
    template <>
    struct hash<Common::Result>
    {
        inline std::size_t operator()(const Common::Result & resultHash ) const
        {
            return resultHash.blackCount * 10 + resultHash.whiteCount;
        }
    };

    template <>
    struct hash<Common::Code>
    {
        inline std::size_t operator()(const Common::Code & codeHash ) const
        {
            size_t returnVal = 0;
            for( int i = 0; i < LengthOfSecret; i++ )
            {
                returnVal += pow(10, i);
            }
            return returnVal;
        }
    };
}

//! Minimax strategy which provides the best results
/*!
    Minimax strategy is most complex strategy I implemented. I think what it does a bit like decision trees in machine learning.
    We do not want find a decision boundary which so precise but have so little elements by other words "overfit".
    The same applies here we find the "WorstCase" in "MiniPart" of this algorithm which helps us our next rule will probably apply more elements.
    In MaxPart we select the most effective one amongst the "WorstCases" to keep a balance between finding very spesific rule which does not apply
    and finding so generic rule which does not eliminate.

    In my UnitTest I run this algorithm 100 times and it averaged to : Average win round with MiniMax strategy was: 4.79
    Meanwhile other algorithm Swaszek was averaging to : Average win round with Swaszek strategy was: 5.82
*/
class MiniMaxStrategy final : public IStrategy
{
public:
    virtual Common::Code Guess(const std::vector<Common::Code>& allCodes, const std::vector<Common::Code>& probableCodes, const std::vector<Common::Code>& pastGuesses) override
    {
        if ( probableCodes.size() == 1 )
            return probableCodes.front();
        if ( pastGuesses.empty() )
            return Common::Code(1122);
        std::unordered_map<Common::Code, int> maximumResultCodeCounts = MiniPart(allCodes, probableCodes, pastGuesses);
        return MaxPart(maximumResultCodeCounts);
    }
private:
    Common::Code MaxPart( const std::unordered_map<Common::Code, int>& maximumResultCodeCounts )
    {
        int minimum = std::numeric_limits<int>::max();
        Common::Code returnVal({1,1,1,1});
        for ( const auto& [key, val] : maximumResultCodeCounts )
        {
            if ( val < minimum )
            {
                minimum = val;
                returnVal = key;
            }
        }
        return returnVal;
    }

    std::unordered_map<Common::Code, int> MiniPart(const std::vector<Common::Code>& allCodes, const std::vector<Common::Code>& probableCodes,
                                                   const std::vector<Common::Code>& pastGuesses)
    {
        std::unordered_map<Common::Code, int> maximumResultCodeCounts;
        for ( const auto& tempCode : allCodes )
        {
            bool isUsed = std::ranges::any_of(pastGuesses, [tempCode]( const Common::Code& in )
            {
                return in == tempCode;
            });
            if ( isUsed )
                continue;

            std::unordered_map<Common::Result, int> resultCounts;
            for ( const auto& allLeftOverCodes : probableCodes )
            {
                auto result = tempCode.Compare(allLeftOverCodes);
                resultCounts[result]++;
            }

            int maximum = 0;
            Common::Code maxKey({1,1,1,1});
            for ( const auto& [key, val] : resultCounts )
            {
                if ( val > maximum )
                {
                    maximum = val;
                    maxKey = tempCode;
                }
            }
            maximumResultCodeCounts[maxKey] = maximum;
        }
        return maximumResultCodeCounts;
    }
};

//! SwaszekStrategy an algorithm which does not provides the best results but no bookkeeping
/*!
    A lovely algorithms which always wins the game. And so easy to implement.

    In my UnitTest I run this algorithm 100 times and it averaged to : Average win round with Swaszek strategy was: 5.82
*/
class SwaszekStrategy final : public IStrategy
{
public:
    virtual Common::Code Guess(const std::vector<Common::Code>& , const std::vector<Common::Code>& probableCodes, const std::vector<Common::Code>& ) override
    {
        return probableCodes.front();
    }
};

//! Human strategy which gets the input from user
/*!
    I was not sure if I should implement this or not but instead asking I just decided to implement it. It helped me in unit tests also.
*/
class HumanStrategy final : public IStrategy
{
public:
    virtual Common::Code Guess(const std::vector<Common::Code>& , const std::vector<Common::Code>&, const std::vector<Common::Code>&  ) override
    {
        int usersGuess;
        std::cout << " Please input your guess to konsole in form of integers.";
        std::cin >> usersGuess;
        return Common::Code(usersGuess);
    }
};

//! UnitTestStrategy which just return a fixed guess for unit tests.
/*!
    This is just for unit tests.
 */
class UnitTestStrategy final : public IStrategy
{
public:
    UnitTestStrategy( const Common::Code& fixedGuess ) : fixedGuess(fixedGuess)
    {
    }
    virtual Common::Code Guess(const std::vector<Common::Code>& , const std::vector<Common::Code>&, const std::vector<Common::Code>&  ) override
    {
        return fixedGuess;
    }
private:
    Common::Code fixedGuess;
};


