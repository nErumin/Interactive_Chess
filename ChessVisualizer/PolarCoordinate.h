#ifndef POLARCOORDINATE_H
#define POLARCOORDINATE_H

struct Vector2;

struct PolarCoordinate
{
public:
    explicit PolarCoordinate(double radius = 0, double degree = 0);

    double getRadius() const noexcept;
    double getDegree() const noexcept;

    void setRadius(double newRadius) noexcept;
    void setDegree(double newDegree) noexcept;

    Vector2 toVector2() const noexcept;
private:
    double radius;
    double degree;
};

#endif // POLARCOORDINATE_H
