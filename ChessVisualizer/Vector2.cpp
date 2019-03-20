#include "Vector2.h"

Vector2::Vector2()
    : Vector2{ 0, 0 }
{

}

Vector2::Vector2(double initX, double initY)
    : coordX{ initX },
      coordY{ initY }
{

}

double Vector2::x() const noexcept
{
    return coordX;
}

double Vector2::y() const noexcept
{
    return coordY;
}

void Vector2::setX(double newX) noexcept
{
    coordX = newX;
}

void Vector2::setY(double newY) noexcept
{
    coordY = newY;
}
