#include "LShapedMovementStrategy.h"
#include "Vector2.h"

LShapedMovementStrategy::LShapedMovementStrategy(PolarCoordinate bodyDirection, PolarCoordinate edgeDirection, bool strategyPermeable)
    : MovementStrategy(strategyPermeable),
      body{ std::move(bodyDirection) },
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

ObstacleMovablePair LShapedMovementStrategy::movableLocationsUsingObstacles(const Vector2& centerLocation, [[maybe_unused]] std::vector<std::deque<bool>> obstacleMap) const noexcept
{
    // no obstacles
    return { {}, movableLocations(centerLocation) };
}
