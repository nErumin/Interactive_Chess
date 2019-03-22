#include "Pawn.h"
#include "Vector2.h"
#include "LineMovementStrategy.h"
#include "PolarCoordinate.h"

Pawn::Pawn(PieceColor initColor)
    : Piece{ initColor },
      diagonalMask{ PawnDiagonalMask::None }
{
    reconstructStrategy();
}

void Pawn::reconstructStrategy()
{
    double radius = isNotMovedYet ? 2.0 : 1.0;

    std::initializer_list<PolarCoordinate> polarList
    {
        PolarCoordinate(radius, 90.0 + getRotationDegree())
    };

    std::vector<std::unique_ptr<MovementStrategy>> strategies;
    strategies.emplace_back(std::make_unique<LineMovementStrategy>(std::move(polarList)));

    if ((diagonalMask & PawnDiagonalMask::Left) != PawnDiagonalMask::None)
    {
        strategies.emplace_back(std::make_unique<LineMovementStrategy>(std::initializer_list<PolarCoordinate>({ PolarCoordinate(1, 135.0 + getRotationDegree() / 2.0) })));
    }

    if ((diagonalMask & PawnDiagonalMask::Right) != PawnDiagonalMask::None)
    {
        strategies.emplace_back(std::make_unique<LineMovementStrategy>(std::initializer_list<PolarCoordinate>({ PolarCoordinate(1, 45.0 + getRotationDegree() * 1.5) })));
    }

    setMovementStratgies(std::move(strategies));
}

PawnDiagonalMask Pawn::getMask() const noexcept
{
    return diagonalMask;
}

void Pawn::markDiagnoalMovable(PawnDiagonalMask mask) noexcept
{
    diagonalMask = mask;
    reconstructStrategy();
}

void Pawn::setPawnMoved(bool isMoved) noexcept
{
    isNotMovedYet = !isMoved;
    reconstructStrategy();
}

void Pawn::setRotationDegree(double newDegree) noexcept
{
    Piece::setRotationDegree(newDegree);
    reconstructStrategy();
}

bool Pawn::isStrategyAcceptable(const std::unique_ptr<MovementStrategy>& strategy) const noexcept
{
    return dynamic_cast<LineMovementStrategy*>(strategy.get()) != nullptr;
}

std::vector<std::pair<PawnDiagonalMask, Vector2>> Pawn::getDiagonalLocations(const Vector2& centerLocation) const
{
    double deltaY = getRotationDegree() >= 179.9999 ? -1.0 : 1.0;

    return {
                std::make_pair(PawnDiagonalMask::Left, centerLocation + Vector2(-1.0, deltaY)),
                std::make_pair(PawnDiagonalMask::Right, centerLocation + Vector2(1.0, deltaY))
           };
}
