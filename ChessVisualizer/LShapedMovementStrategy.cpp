#include "LShapedMovementStrategy.h"
#include "Vector2.h"

LShapedMovementStrategy::LShapedMovementStrategy(PolarCoordinate bodyDirection, PolarCoordinate edgeDirection)
    : body{ std::move(bodyDirection) },
      edge{ std::move(edgeDirection) }
{

}

std::vector<Vector2> LShapedMovementStrategy::movableLocations(const Vector2& centerLocation) const noexcept
{
    std::vector<Vector2> locations;

    Vector2 edgeStartLocations = centerLocation + body.toVector2();
    locations.push_back(edgeStartLocations + edge.toVector2());

    return locations;
};