#include "PolarCoordinate.h"
#include "Vector2.h"
#include "MathUtils.h"

PolarCoordinate::PolarCoordinate(double initRadius, double initDegree)
    : radius{ initRadius },
      degree{ initDegree }
{

}

double PolarCoordinate::getRadius() const noexcept
{
    return radius;
}

double PolarCoordinate::getDegree() const noexcept
{
    return degree;
}

void PolarCoordinate::setRadius(double newRadius) noexcept
{
    radius = newRadius;
}

void PolarCoordinate::setDegree(double newDegree) noexcept
{
    degree = newDegree;
}

Vector2 PolarCoordinate::toVector2() const noexcept
{
    double radian = degreeToRadian(degree);
    return { radius * std::cos(radian) , radius * std::sin(radian) };
}
