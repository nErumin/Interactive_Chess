#ifndef PIECE_H
#define PIECE_H

#include <vector>
#include <initializer_list>
#include "piececolor.h"

class MovementStrategy;
class Vector2;

class Piece
{
public:
    explicit Piece(PieceColor color = PieceColor::None);

    std::vector<Vector2> movableLocations() const noexcept;
    double getRotationDegree() const noexcept;
    PieceColor getColor() const noexcept;

    void setRotationDegree(double newDegree) noexcept;
    void setColor(PieceColor newColor) noexcept;
    void setMovementStratgies(std::initializer_list<MovementStrategy> newStrategies) noexcept;
private:
    std::vector<MovementStrategy> movementStrategies;
    PieceColor color;
    double rotationDegree;
};

#endif // PIECE_H
