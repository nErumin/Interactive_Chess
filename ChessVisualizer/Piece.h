#ifndef PIECE_H
#define PIECE_H

#include <memory>
#include <vector>
#include <initializer_list>
#include "PieceColor.h"

class MovementStrategy;
struct Vector2;

class Piece
{
public:
    explicit Piece(PieceColor color = PieceColor::None);

    std::vector<Vector2> movableLocations(const Vector2& pieceLocation) const;
    std::vector<Vector2> movableLocationsUsingObstacles(const Vector2& pieceLocation, const std::vector<std::vector<PieceColor>>& obstacleMap) const;
    double getRotationDegree() const noexcept;
    PieceColor getColor() const noexcept;

    virtual void setRotationDegree(double newDegree) noexcept;
    void setColor(PieceColor newColor) noexcept;
    void setMovementStratgies(std::vector<std::unique_ptr<MovementStrategy>>&& newStrategies);

    virtual ~Piece();
protected:
    virtual bool isStrategyAcceptable(const std::unique_ptr<MovementStrategy>& strategy) const noexcept = 0;
private:
    std::vector<std::unique_ptr<MovementStrategy>> movementStrategies;
    PieceColor color;
    double rotationDegree;
};

#endif // PIECE_H
