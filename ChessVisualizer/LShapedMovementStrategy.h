#ifndef LSHAPEDMOVEMENTSTRATEGY_H
#define LSHAPEDMOVEMENTSTRATEGY_H

#include "MovementStrategy.h"
#include "PolarCoordinate.h"

class LShapedMovementStrategy : public MovementStrategy
{
public:
    explicit LShapedMovementStrategy(PolarCoordinate bodyDirection, PolarCoordinate edgeDirection);
    std::vector<Vector2> movableLocations(const Vector2& centerLocation) const noexcept override;

private:
    PolarCoordinate body;
    PolarCoordinate edge;
};

#endif // LSHAPEDMOVEMENTSTRATEGY_H
