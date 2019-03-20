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
private:
    double coordX;
    double coordY;
};

#endif // VECTOR2_H
