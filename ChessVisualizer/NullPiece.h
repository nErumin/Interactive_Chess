#ifndef NULLPIECE_H
#define NULLPIECE_H

#include "Piece.h"

enum class PieceColor;

class NullPiece : public Piece
{
public:
    NullPiece() = default;
    ~NullPiece() override = default;
protected:
    virtual bool isStrategyAcceptable(const std::unique_ptr<MovementStrategy>& strategy) const noexcept override
    {
        return false;
    }
};

#endif // NULLPIECE_H
