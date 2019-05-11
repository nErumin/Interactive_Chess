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

inline constexpr PieceColor operator&(const PieceColor& lhs, const PieceColor& rhs)
{
    auto lhsInteger = static_cast<size_t>(lhs);
    auto rhsInteger = static_cast<size_t>(rhs);

    return static_cast<PieceColor>(lhsInteger & rhsInteger);
}

inline constexpr PieceColor operator|(const PieceColor& lhs, const PieceColor& rhs)
{
    auto lhsInteger = static_cast<size_t>(lhs);
    auto rhsInteger = static_cast<size_t>(rhs);

    return static_cast<PieceColor>(lhsInteger | rhsInteger);
}

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

inline PieceColor getEnemyColor(PieceColor pieceColor)
{
    switch (pieceColor)
    {
        case PieceColor::Black:
            return PieceColor::White;
        case PieceColor::White:
            return PieceColor::Black;
        default:
            return PieceColor::None;
    }
}

#endif
