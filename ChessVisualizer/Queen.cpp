#include "Queen.h"
#include "LineMovementStrategy.h"
#include "PolarCoordinate.h"

Queen::Queen(PieceColor initColor)
    : Piece{ initColor }
{
    std::initializer_list<PolarCoordinate> polarList
    {
        PolarCoordinate(7, 0),
        PolarCoordinate(7, 45),
        PolarCoordinate(7, 90),
        PolarCoordinate(7, 135),
        PolarCoordinate(7, 180),
        PolarCoordinate(7, 225),
        PolarCoordinate(7, 270),
        PolarCoordinate(7, 315),
    };

    std::vector<std::unique_ptr<MovementStrategy>> strategies;
    strategies.emplace_back(std::make_unique<LineMovementStrategy>(std::move(polarList)));

    setMovementStratgies(std::move(strategies));
}

bool Queen::isStrategyAcceptable(const std::unique_ptr<MovementStrategy>& strategy) const noexcept
{
    return dynamic_cast<LineMovementStrategy*>(strategy.get()) != nullptr;
}
