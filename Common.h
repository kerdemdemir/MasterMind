#pragma once

#include <algorithm>
#include <random>
#include <ranges>
#include <span>

constexpr int LengthOfSecret = 4;
constexpr int ColorCount     = 6;
constexpr int MaximumRoundCount = 10;

namespace Common
{
    //! GameMode
    /*!
        Game currently has three modes. It can be played as a Human. In this case user will give it guesses by stdin.
        Or there are two algorithms which will be using by computer.
    */
    enum class GameMode
    {
        Human = 0,
        MiniMax,
        Swaszek
    };

    //! Result
    /*!
        According to wikipedia there are two outcomes after an guess.
        If both position and color is right than codekeeper should add a black peg.
        If only position is correct than only a white peg should be added.
    */
    struct Result
    {
        int blackCount = 0;
        int whiteCount = 0;

        bool operator==( const Result& rhs ) const
        {
            return rhs.blackCount == blackCount && rhs.whiteCount == whiteCount ;
        }

        std::string ToString()
        {
            return std::to_string(blackCount) + " black " + std::to_string(whiteCount) + " white";
        }
    };

    //! Code Encapsulates the all important code
    /*!
        This class encapsulate all important code. It is often some actions are repeated in this game by different entities like CodeBreaker,
        CodeKeeper. That is why those functions are encapsulate in this all accessible struct.
        Most important functions is provides are
        1 - Compare function which return a result after comparing codes
        2 - This struct provides functions to iterate to next code example 1112-->1113.Please see unit tests for more.
        3 - Convenient CTORs which accepts array, integer or generates a random code when nothing provided.
    */
    struct Code
    {
        Code()
        {
            GenerateRandomCode();
        }

        Code( std::array<int,LengthOfSecret> colorCodeList )
        {
            code = colorCodeList;
        }

        Code( int integerCode )
        {
            for ( int i = 0; i < LengthOfSecret; i++ )
            {
                int moduler = pow(10, i+1);
                int divider = moduler/10;
                int currentDigit = integerCode % moduler / divider;
                code[i] = currentDigit;
            }
            std::reverse(code.begin(), code.end());
        }

        bool operator==( const Code& rhs ) const
        {
            return rhs.code == code;
        }

        Result Compare( const Code& rhs ) const
        {
            Result returnVal;
            const auto& compareCode = rhs.GetCode();
            for ( size_t i = 0; i < LengthOfSecret; i++ )
            {
                int compareNumber = compareCode[i];
                if ( compareNumber == code[i] )
                {
                    returnVal.blackCount++;
                }
            }

            auto sortedSelfCopy = code;
            auto sortedCompareCopy = compareCode;

            std::ranges::sort(sortedSelfCopy);
            std::ranges::sort(sortedCompareCopy);

            std::vector<int> intersectionSet;
            std::set_intersection(sortedSelfCopy.begin(), sortedSelfCopy.end(),
                                  sortedCompareCopy.begin(), sortedCompareCopy.end(), std::back_inserter(intersectionSet));

            returnVal.whiteCount = intersectionSet.size() - returnVal.blackCount;
            return returnVal;
        }

        std::array<int,LengthOfSecret> NextCode() const
        {
            std::array<int,LengthOfSecret> returnVal = code;
            if ( code.back() != ColorCount)
            {
                returnVal[LengthOfSecret-1] += 1;
                return returnVal;
            }

            bool isLastElem = std::all_of(code.begin(), code.end(), []( int elem ){
                return elem == ColorCount;
            });
            if ( isLastElem )
                return returnVal;

            IncreaseCodeIndex(returnVal, LengthOfSecret-1);
            return returnVal;
        }

        void IncreaseCodeIndex( std::array<int,LengthOfSecret>& code, int index ) const
        {
            if( code[index] == ColorCount )
            {
                code[index] = 1;
                IncreaseCodeIndex(code, index-1);
            }
            else
                ++code[index];
        }

        std::string ToString() const
        {
            std::string out;
            for ( auto elem : code )
            {
                out.append("*" + std::to_string(elem) + "*");
            }
            return out;
        }

        const std::array<int,LengthOfSecret>& GetCode() const
        {
            return code;
        }

    private:
        void GenerateRandomCode()
        {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> distrib(1, ColorCount);
            for ( size_t i = 0; i < LengthOfSecret; i++ )
            {
                code[i] = distrib(gen);
            }
        }

        std::array<int,LengthOfSecret> code;
    };

    //! GetStartingInteger
    /*!
        The user input starts from 1111 or 11111 or something like 1111.. depending on the length of secret.
        This function calculates this number.
    */
    int GetStartingInteger() {
        int returnVal = 0;
        for ( int i = 0; i < LengthOfSecret; i++ )
            returnVal += pow(10, i);
        return returnVal;
    }
}
