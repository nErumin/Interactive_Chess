#include "Pawn.h"
#include "LineMovementStrategy.h"
#include "PolarCoordinate.h"

Pawn::Pawn(PieceColor initColor)
    : Piece{ initColor }
{
    std::initializer_list<PolarCoordinate> polarList
    {
        PolarCoordinate(2, 90)
    };

    std::vector<std::unique_ptr<MovementStrategy>> strategies;
    strategies.emplace_back(std::make_unique<LineMovementStrategy>(std::move(polarList)));

    setMovementStratgies(std::move(strategies));
}

bool Pawn::isStrategyAcceptable(const std::unique_ptr<MovementStrategy>& strategy) const noexcept
{
    return dynamic_cast<LineMovementStrategy*>(strategy.get()) != nullptr;
}
