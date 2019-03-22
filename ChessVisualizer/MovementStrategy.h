#ifndef MOVEMENTSTRATEGY_H
#define MOVEMENTSTRATEGY_H

#include <vector>
#include <deque>

struct Vector2;
using ObstacleMovablePair = std::pair<std::vector<Vector2>, std::vector<Vector2>>;

class MovementStrategy
{
public:
    virtual std::vector<Vector2> movableLocations(const Vector2& centerLocation) const noexcept = 0;
    virtual ObstacleMovablePair movableLocationsUsingObstacles(const Vector2& centerLocation, std::vector<std::deque<bool>> obstacleMap) const noexcept = 0;
    virtual ~MovementStrategy() = default;
};

#endif // MOVEMENTSTRATEGY_H
