#pragma once

#include "doctest.h"
#include "../Common.h"
#include "../Game.h"

TEST_CASE("Testing random code(secret) generation") {
    Common::Code code;
    for ( auto number : code.GetCode() )
        CHECK(number!=0);
}

TEST_CASE("Testing comparison between two codes") {
    Common::Code code( {1,2,3,4 });
    Common::Code codeRhs( {1,2,3,4 });
    auto result = code.Compare(codeRhs);
    CHECK(result.blackCount == 4);
    CHECK(result.whiteCount == 0);

    Common::Code code2( {4,3,2,1});
    Common::Code code2Rhs( {1,2,3,4});
    result = code2.Compare(code2Rhs);
    CHECK(result.blackCount == 0);
    CHECK(result.whiteCount == 4);

    Common::Code code3( {4,4,2,1});
    Common::Code code3Rhs( {1,2,3,4});
    result = code3.Compare(code3Rhs);
    CHECK(result.blackCount == 0);
    CHECK(result.whiteCount == 3);

    Common::Code code4( {4,4,2,1});
    Common::Code code4Rhs( {4,2,3,4});
    result = code4.Compare(code4Rhs);
    CHECK(result.blackCount == 1);
    CHECK(result.whiteCount == 2);
}

TEST_CASE("Testing integer ctor of Code") {
    Common::Code code( 1234 );
    Common::Code codeRhs( {1,2,3,4 });
    auto result = code.Compare(codeRhs);
    CHECK(result.blackCount == 4);
    CHECK(result.whiteCount == 0);
}

TEST_CASE("Testing iterating to next code from current code") {
    Common::Code code( 1234 );
    Common::Code codeRhs( {1,2,3,5 });
    Common::Code nextCode = Common::Code(code.NextCode());
    auto result = nextCode.Compare(codeRhs);
    CHECK(result.blackCount == 4);

    Common::Code code2( 1236 );
    Common::Code codeRhs2( {1,2,4,1 });
    Common::Code nextCode2 = Common::Code(code2.NextCode());
    result = nextCode2.Compare(codeRhs2);
    CHECK(result.blackCount == 4);

    Common::Code code3( 1266 );
    Common::Code codeRhs3( {1,3,1,1 });
    Common::Code nextCode3 = Common::Code(code3.NextCode());
    result = nextCode3.Compare(codeRhs3);
    CHECK(result.blackCount == 4);

    Common::Code code4( 6666 );
    Common::Code codeRhs4( {6,6,6,6 });
    Common::Code nextCode4 = Common::Code(code4.NextCode());
    result = nextCode4.Compare(codeRhs4);
    CHECK(result.blackCount == 4);
}

TEST_CASE("Testing if we generated all outcomes correctly") {
    Game game( Common::GameMode::Human );
    auto allPossibleCodes = game.GenerateAllPossibleCodes();
    CHECK(allPossibleCodes.size() == 1296);//Wikipedia says there should be 1296
}

TEST_CASE("Testing if elimination step is correct") {
    CodeBreaker codeBreaker( std::make_shared<UnitTestStrategy>(Common::Code(5566)) );
    Common::Code code1( 6666 );
    Common::Code code2( 5555 );
    Common::Code code3( 4444 );
    codeBreaker.SetAllCodes( { code1, code2, code3});
    codeBreaker.Guess();
    int countOfLeftOverElems = codeBreaker.SetResult(Common::Result{2,0});
    //It is too hard to test this case. Within this simple case 6666 and 5555 should stay 4444 should be removed total number should be 2
    CHECK(countOfLeftOverElems == 2);
}

TEST_CASE("Testing game with SwaszekStrategy strategy") {
    Game game( Common::GameMode::Swaszek );
    int winRound = game.StartTheGame();
    CHECK(winRound != -1);
}

TEST_CASE("Testing the performace of game with Swaszek strategy") {
    int totalWinTurnCount = 0;
    std::cout << "Checking average win turn for swaszek strategy" << std::endl;
    for ( int i = 0; i < 100; i++ )
    {
        Game game( Common::GameMode::Swaszek );
        int winRound = game.StartTheGame();
        if(winRound != -1)
            totalWinTurnCount += (winRound + 1);
    }

    double average = totalWinTurnCount / 100.0;
    std::cout << "Average win round with Swaszek strategy was: " << average << std::endl;
}

TEST_CASE("Testing the performace of game with MiniMax strategy") {
    int totalWinTurnCount = 0;
    std::cout << "Checking average win turn for miniMax strategy" << std::endl;
    for ( int i = 0; i < 100; i++ )
    {
        Game game( Common::GameMode::MiniMax );
        int winRound = game.StartTheGame();
        totalWinTurnCount += (winRound+1);
        CHECK(winRound < 5);
    }

    double average = totalWinTurnCount / 100.0;
    std::cout << "Average win round with MiniMax strategy was: " << average << std::endl;
}