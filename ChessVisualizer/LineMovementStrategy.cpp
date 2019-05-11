#include "LineMovementStrategy.h"
#include "PolarCoordinate.h"
#include "MathUtils.h"
#include "Vector2.h"
#include <vector>

inline double discritizeLength(double length)
{
    double discritizedValue = 0.0;

    if (std::abs(length) >= 0.5)
    {
        discritizedValue = length < 0.0 ? -1.0 : 1.0;
    }

    return discritizedValue;
}

inline Vector2 discritizePolarCoordinate(const PolarCoordinate& polarCoordinate)
{
    auto vec = polarCoordinate.toVector2();

    double deltaX = discritizeLength(vec.x());
    double deltaY = discritizeLength(vec.y());

    return { deltaX, deltaY };
}

LineMovementStrategy::LineMovementStrategy(std::initializer_list<PolarCoordinate> usedDirections, bool strategyPermeable)
    : MovementStrategy(strategyPermeable),
      directions(usedDirections)
{
}

std::vector<Vector2> LineMovementStrategy::movableLocations(const Vector2& centerLocation) const noexcept
{
    std::vector<Vector2> locations;

    for (const auto& direction : directions)
    {
        auto deltaVector = discritizePolarCoordinate(direction);
        int directionLength = static_cast<int>(direction.getRadius());

        for (int length = 1; length <= directionLength; ++length)
        {
            locations.emplace_back(centerLocation.x() + deltaVector.x() * length, centerLocation.y() + deltaVector.y() * length);
        }
    }

    return locations;
}

ObstacleMovablePair LineMovementStrategy::movableLocationsUsingObstacles(const Vector2& centerLocation, std::vector<std::deque<bool>> obstacleMap) const noexcept
{
    std::vector<Vector2> obstacles;
    std::vector<Vector2> locations;

    for (const auto& direction : directions)
    {
        auto deltaVector = discritizePolarCoordinate(direction);
        int directionLength = static_cast<int>(direction.getRadius());

        for (int length = 1; length <= directionLength; ++length)
        {
            Vector2 nextLocation{ centerLocation.x() + deltaVector.x() * length, centerLocation.y() + deltaVector.y() * length };
            auto normalizedNextLocation = normalizeToIntegerVector(nextLocation);

            // are there any obstacles?
            if (normalizedNextLocation.first < obstacleMap.size() &&
                normalizedNextLocation.second < obstacleMap.front().size() &&
                obstacleMap[normalizedNextLocation.first][normalizedNextLocation.second])
            {
                obstacles.push_back(nextLocation);
                break;
            }

            locations.push_back(nextLocation);
        }
    }

    return { obstacles, locations };
}
