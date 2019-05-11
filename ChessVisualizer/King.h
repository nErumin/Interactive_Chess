#ifndef KING_H
#define KING_H

#include "Piece.h"

enum class PieceColor;

class King : public Piece
{
public:
    King(PieceColor initColor);

    size_t getPriority() const noexcept override { return 100; }
    ~King() override = default;
protected:
    virtual bool isStrategyAcceptable(const std::unique_ptr<MovementStrategy>& strategy) const noexcept override;
};

#endif // KING_H
