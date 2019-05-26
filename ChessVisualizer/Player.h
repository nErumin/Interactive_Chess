#ifndef PLAYER_H
#define PLAYER_H

#include "PlayerType.h"
#include "PieceColor.h"
#include "Timer.h"

class Player final
{
public:
    explicit Player(PlayerType initialType = PlayerType::None, PieceColor ownColor = PieceColor::None);

    PlayerType getType() const noexcept;
    PieceColor getOwningPieceColor() const noexcept;
    void setOwningPieceColor(PieceColor newColor) noexcept;

    Timer& getTimer() noexcept;
private:
    PlayerType type;
    PieceColor owningPieceColor;
    Timer timer;
};

#endif // PLAYER_H
