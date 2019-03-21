#ifndef KNIGHT_H
#define KNIGHT_H

#include "Piece.h"

enum class PieceColor;

class Knight : public Piece
{
public:
    Knight(PieceColor initColor);

    ~Knight() override = default;
protected:
    virtual bool isStrategyAcceptable(const std::unique_ptr<MovementStrategy>& strategy) const noexcept override;
};

#endif // KNIGHT_H
