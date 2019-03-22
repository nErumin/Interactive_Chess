#include "Player.h"
#include "PieceColor.h"
#include "PlayerType.h"

Player::Player(PlayerType initialType, PieceColor ownColor)
    : type{ initialType },
      owningPieceColor{ ownColor }
{

}

PlayerType Player::getType() const noexcept
{
    return type;
}

PieceColor Player::getOwningPieceColor() const noexcept
{
    return owningPieceColor;
}
