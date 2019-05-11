#include "King.h"
#include "LineMovementStrategy.h"
#include "PolarCoordinate.h"

King::King(PieceColor initColor)
    : Piece{ initColor }
{
    std::initializer_list<PolarCoordinate> polarList
    {
        PolarCoordinate(1, 0),
        PolarCoordinate(1, 45),
        PolarCoordinate(1, 90),
        PolarCoordinate(1, 135),
        PolarCoordinate(1, 180),
        PolarCoordinate(1, 225),
        PolarCoordinate(1, 270),
        PolarCoordinate(1, 315),
    };

    std::vector<std::unique_ptr<MovementStrategy>> strategies;
    strategies.emplace_back(std::make_unique<LineMovementStrategy>(std::move(polarList)));

    setMovementStratgies(std::move(strategies));
}

bool King::isStrategyAcceptable(const std::unique_ptr<MovementStrategy>& strategy) const noexcept
{
    return dynamic_cast<LineMovementStrategy*>(strategy.get()) != nullptr;
}
