#include "Player.h"
#include "PieceColor.h"
#include "PlayerType.h"
#include "Timer.h"

Player::Player(PlayerType initialType, PieceColor ownColor)
    : type{ initialType },
      owningPieceColor{ ownColor }
{

}

Timer& Player::getTimer() noexcept
{
    return timer;
}

PlayerType Player::getType() const noexcept
{
    return type;
}

PieceColor Player::getOwningPieceColor() const noexcept
{
    return owningPieceColor;
}
