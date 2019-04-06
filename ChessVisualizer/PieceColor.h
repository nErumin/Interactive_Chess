#ifndef PIECECOLOR_H
#define PIECECOLOR_H

#include "MathUtils.h"
#include <utility>

enum class PieceColor
{
    None = 0,
    Black = 1,
    White = 2
};

inline std::pair<PieceColor, PieceColor> pickRandomColorPair()
{
    if (pickRandomNumber(0, 2) == 0)
    {
        return { PieceColor::Black, PieceColor::White };
    }
    else
    {
        return { PieceColor::White, PieceColor::Black };
    }
}

#endif
