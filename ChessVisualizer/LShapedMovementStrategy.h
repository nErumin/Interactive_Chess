#ifndef LSHAPEDMOVEMENTSTRATEGY_H
#define LSHAPEDMOVEMENTSTRATEGY_H

#include "MovementStrategy.h"
#include "PolarCoordinate.h"

class LShapedMovementStrategy : public MovementStrategy
{
public:
    explicit LShapedMovementStrategy(PolarCoordinate bodyDirection, PolarCoordinate edgeDirection, bool strategyPermeable = true);
    ObstacleMovablePair movableLocationsUsingObstacles(const Vector2& centerLocation, std::vector<std::deque<bool>> obstacleMap) const noexcept override;

    std::vector<Vector2> movableLocations(const Vector2& centerLocation) const noexcept override;
private:
    PolarCoordinate body;
    PolarCoordinate edge;
};

#endif // LSHAPEDMOVEMENTSTRATEGY_H
