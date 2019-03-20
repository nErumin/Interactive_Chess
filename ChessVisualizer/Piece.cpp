#include <exception>
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
        locations.emplace_back(movables.begin(), movables.end());
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

void Piece::setMovementStratgies(std::initializer_list<std::unique_ptr<MovementStrategy>> newStrategies)
{
    movementStrategies.assign(newStrategies);
}

Piece::~Piece()
{

}
