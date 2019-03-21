#ifndef BISHOP_H
#define BISHOP_H

#include "Piece.h"

enum class PieceColor;

class Bishop : public Piece
{
public:
    Bishop(PieceColor initColor);

    ~Bishop() override = default;
protected:
    virtual bool isStrategyAcceptable(const std::unique_ptr<MovementStrategy>& strategy) const noexcept override;
};

#endif // BISHOP_H
