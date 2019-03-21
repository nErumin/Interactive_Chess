#ifndef LINEMOVEMENTSTRATEGY_H
#define LINEMOVEMENTSTRATEGY_H

#include "MovementStrategy.h"
#include <vector>
#include <initializer_list>

struct PolarCoordinate;

class LineMovementStrategy : public MovementStrategy
{
public:
    explicit LineMovementStrategy(std::initializer_list<PolarCoordinate> usedDirections);
    std::vector<Vector2> movableLocations(const Vector2& centerLocation) const noexcept override;

    ~LineMovementStrategy() override = default;
private:
    std::vector<PolarCoordinate> directions;
};


#endif // LINEMOVEMENTSTRATEGY_H
