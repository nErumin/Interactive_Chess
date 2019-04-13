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

ObstacleMovablePair LShapedMovementStrategy::movableLocationsUsingObstacles(const Vector2& centerLocation, std::vector<std::deque<bool>> obstacleMap) const noexcept
{
    std::vector<Vector2> obstacles;
    std::vector<Vector2> locations;

    auto movable = movableLocations(centerLocation);

    for (const auto& location : movable)
    {
        auto normalized = normalizeToIntegerVector(location);

        if (normalized.first < obstacleMap.size() &&
            normalized.second < obstacleMap.front().size() &&
            obstacleMap[normalized.first][normalized.second])
        {
            obstacles.push_back(location);
        }
        else
        {
            locations.push_back(location);
        }
    }

    return { obstacles, locations };
}
