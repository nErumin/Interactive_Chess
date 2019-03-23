#ifndef QUEEN_H
#define QUEEN_H

#include "Piece.h"

enum class PieceColor;

class Queen : public Piece
{
public:
    Queen(PieceColor initColor);

    ~Queen() override = default;
protected:
    virtual bool isStrategyAcceptable(const std::unique_ptr<MovementStrategy>& strategy) const noexcept override;
};

#endif // QUEEN_H
