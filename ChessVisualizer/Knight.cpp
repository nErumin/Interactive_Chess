#include "Knight.h"
#include "LShapedMovementStrategy.h"
#include "PolarCoordinate.h"

Knight::Knight(PieceColor initColor)
    : Piece{ initColor }
{
    std::vector<std::unique_ptr<MovementStrategy>> strategies;

    strategies.emplace_back(std::make_unique<LShapedMovementStrategy>(PolarCoordinate{ 2, 0 }, PolarCoordinate{ 1, 90 }));
    strategies.emplace_back(std::make_unique<LShapedMovementStrategy>(PolarCoordinate{ 2, 0 }, PolarCoordinate{ 1, 270 }));
    strategies.emplace_back(std::make_unique<LShapedMovementStrategy>(PolarCoordinate{ 2, 270 }, PolarCoordinate{ 1, 0 }));
    strategies.emplace_back(std::make_unique<LShapedMovementStrategy>(PolarCoordinate{ 2, 270 }, PolarCoordinate{ 1, 180 }));
    strategies.emplace_back(std::make_unique<LShapedMovementStrategy>(PolarCoordinate{ 2, 180 }, PolarCoordinate{ 1, 90 }));
    strategies.emplace_back(std::make_unique<LShapedMovementStrategy>(PolarCoordinate{ 2, 180 }, PolarCoordinate{ 1, 270 }));
    strategies.emplace_back(std::make_unique<LShapedMovementStrategy>(PolarCoordinate{ 2, 90 }, PolarCoordinate{ 1, 0 }));
    strategies.emplace_back(std::make_unique<LShapedMovementStrategy>(PolarCoordinate{ 2, 90 }, PolarCoordinate{ 1, 180 }));

    setMovementStratgies(std::move(strategies));
}

bool Knight::isStrategyAcceptable(const std::unique_ptr<MovementStrategy>& strategy) const noexcept
{
    return dynamic_cast<LShapedMovementStrategy*>(strategy.get()) != nullptr;
}
