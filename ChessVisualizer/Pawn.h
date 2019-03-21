#ifndef PAWN_H
#define PAWN_H

#include "Piece.h"

enum class PieceColor;

class Pawn : public Piece
{
public:
    Pawn(PieceColor initColor);

    ~Pawn() override = default;
protected:
    virtual bool isStrategyAcceptable(const std::unique_ptr<MovementStrategy>& strategy) const noexcept override;
};

#endif // PAWN_H
