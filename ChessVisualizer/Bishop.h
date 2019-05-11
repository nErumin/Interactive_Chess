#ifndef BISHOP_H
#define BISHOP_H

#include "Piece.h"

enum class PieceColor;

class Bishop : public Piece
{
public:
    Bishop(PieceColor initColor);

    size_t getPriority() const noexcept override { return 5; }
    ~Bishop() override = default;
protected:
    virtual bool isStrategyAcceptable(const std::unique_ptr<MovementStrategy>& strategy) const noexcept override;
};

#endif // BISHOP_H
