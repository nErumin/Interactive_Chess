#ifndef VECTOR2_H
#define VECTOR2_H

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

Vector2 operator+(const Vector2& lhs, const Vector2& rhs) noexcept;

#endif // VECTOR2_H
