#pragma once

#include "Common.h"

//! CodeMaker is the keeper of the secret and reveals the results to CodeBreaker
/*!
    Code maker encapsulates the all important secretCode. SecretCode is random each time which takes advantage of CTOR of CommonCode.
    And it compares its secret code by given guess.
*/
class CodeMaker
{
public:
    Common::Result GetResultOfGuess( Common::Code guessCode )
    {
        return secretCode.Compare(guessCode);
    }

    const Common::Code& GetSecretCode() const
    {
        return secretCode;
    }
private:
    Common::Code secretCode;
};