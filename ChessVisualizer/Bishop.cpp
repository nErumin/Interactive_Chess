#include "LineMovementStrategy.h"
#include "PolarCoordinate.h"
#include "Bishop.h"

Bishop::Bishop(PieceColor initColor)
    : Piece{ initColor }
{
    std::initializer_list<PolarCoordinate> polarList
    {
        PolarCoordinate(7, 45),
        PolarCoordinate(7, 135),
        PolarCoordinate(7, 225),
        PolarCoordinate(7, 315)
    };

    std::vector<std::unique_ptr<MovementStrategy>> strategies;
    strategies.emplace_back(std::make_unique<LineMovementStrategy>(std::move(polarList)));

    setMovementStratgies(std::move(strategies));
}

bool Bishop::isStrategyAcceptable(const std::unique_ptr<MovementStrategy>& strategy) const noexcept
{
    return dynamic_cast<LineMovementStrategy*>(strategy.get()) != nullptr;
}
