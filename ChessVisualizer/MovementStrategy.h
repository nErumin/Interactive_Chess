#ifndef MOVEMENTSTRATEGY_H
#define MOVEMENTSTRATEGY_H

#include <vector>

struct Vector2;

class MovementStrategy
{
public:
    virtual std::vector<Vector2> movableLocations(const Vector2& centerLocation) const noexcept = 0;
    virtual ~MovementStrategy();
};

#endif // MOVEMENTSTRATEGY_H
