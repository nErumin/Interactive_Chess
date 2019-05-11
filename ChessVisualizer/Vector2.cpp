#include "Vector2.h"
#include <cmath>

std::ostream& operator<<(std::ostream& stream, const Vector2& vec)
{
    stream << "(" << vec.x() << ", " << vec.y() << ")";
    return stream;
}

bool operator<(const Vector2& lhs, const Vector2& rhs)
{
    return (lhs.magitude() < rhs.magitude());
}

Vector2 operator+(const Vector2& lhs, const Vector2& rhs)
{
    return { lhs.x() + rhs.x(), lhs.y() + rhs.y() };
}

Vector2 operator-(const Vector2& lhs, const Vector2& rhs)
{
    return { lhs.x() - rhs.x(), lhs.y() - rhs.y() };
}

bool operator==(const Vector2& lhs, const Vector2& rhs)
{
    return (std::abs(lhs.x() - rhs.x()) < 0.0001) &&
           (std::abs(lhs.y() - rhs.y()) < 0.0001);
}

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

Vector2 Vector2::normalize() const noexcept
{
    double mag = magitude();

    if (mag < 0.0000001)
    {
        return { 0.0, 0.0 };
    }

    return { coordX / mag, coordY / mag };
}

double Vector2::magitude() const noexcept
{
    return std::sqrt(coordX * coordX + coordY * coordY);
}
