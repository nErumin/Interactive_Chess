#ifndef PLAYER_H
#define PLAYER_H

#include "PlayerType.h"
#include "PieceColor.h"

class Player final
{
public:
    explicit Player(PlayerType initialType = PlayerType::None, PieceColor ownColor = PieceColor::None);

    PlayerType getType() const noexcept;
    PieceColor getOwningPieceColor() const noexcept;

private:
    PlayerType type;
    PieceColor owningPieceColor;
};

#endif // PLAYER_H
