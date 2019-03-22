#ifndef VECTOR2_H
#define VECTOR2_H

#include <utility>
#include "MathUtils.h"

struct Vector2
{
public:
    Vector2();
    Vector2(double initX, double initY);

    double x() const noexcept;
    double y() const noexcept;

    void setX(double newCoordX) noexcept;
    void setY(double newCoordY) noexcept;

    Vector2 normalize() const noexcept;
    double magitude() const noexcept;
private:
    double coordX;
    double coordY;
};

Vector2 operator+(const Vector2& lhs, const Vector2& rhs);
Vector2 operator-(const Vector2& lhs, const Vector2& rhs);
bool operator==(const Vector2& lhs, const Vector2& rhs);

inline std::pair<size_t, size_t> normalizeToIntegerVector(const Vector2& vec)
{
    return { static_cast<size_t>(std::round(vec.x())), static_cast<size_t>(std::round(vec.y()))};
}

#endif // VECTOR2_H
