#ifndef ROOK_H
#define ROOK_H

#include "Piece.h"

enum class PieceColor;

class Rook : public Piece
{
public:
    Rook(PieceColor initColor);

    ~Rook() override = default;
protected:
    virtual bool isStrategyAcceptable(const std::unique_ptr<MovementStrategy>& strategy) const noexcept override;
};

#endif // ROOK_H
