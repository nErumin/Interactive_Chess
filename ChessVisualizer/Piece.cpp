#include <exception>
#include <iterator>
#include <algorithm>

#include "Piece.h"
#include "MovementStrategy.h"
#include "Vector2.h"

Piece::Piece(PieceColor initColor)
    : color{ initColor },
      rotationDegree{ 0.0 }
{
}

std::vector<Vector2> Piece::movableLocations(const Vector2& pieceLocation) const
{
    std::vector<Vector2> locations;

    for (const auto& strategy : movementStrategies)
    {
        if (!isStrategyAcceptable(strategy))
        {
            throw std::logic_error{ "contains non-acceptable strategy" };
        }

        auto movables = strategy->movableLocations(pieceLocation);

        std::copy(movables.begin(), movables.end(), std::back_inserter(locations));
    }

    return locations;
}

double Piece::getRotationDegree() const noexcept
{
    return rotationDegree;
}

PieceColor Piece::getColor() const noexcept
{
    return color;
}

void Piece::setRotationDegree(double newDegree) noexcept
{
    rotationDegree = newDegree;
}

void Piece::setColor(PieceColor newColor) noexcept
{
    color = newColor;
}

void Piece::setMovementStratgies(std::vector<std::unique_ptr<MovementStrategy>>&& newStrategies)
{
    movementStrategies.clear();

    for (auto& strategy : newStrategies)
    {
        movementStrategies.push_back(std::move(strategy));
    }
}

Piece::~Piece()
{

}
