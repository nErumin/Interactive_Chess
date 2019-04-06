#ifndef MOVEMENTSTRATEGY_H
#define MOVEMENTSTRATEGY_H

#include <vector>
#include <deque>

struct Vector2;
using ObstacleMovablePair = std::pair<std::vector<Vector2>, std::vector<Vector2>>;

class MovementStrategy
{
public:
    explicit MovementStrategy(bool strategyPermeable = true)
        : permeable{ strategyPermeable }
    {
    }

    bool isPermeable() const noexcept { return permeable; }
    virtual std::vector<Vector2> movableLocations(const Vector2& centerLocation) const noexcept = 0;
    virtual ObstacleMovablePair movableLocationsUsingObstacles(const Vector2& centerLocation, std::vector<std::deque<bool>> obstacleMap) const noexcept = 0;
    virtual ~MovementStrategy() = default;
private:
    const bool permeable;
};

#endif // MOVEMENTSTRATEGY_H
