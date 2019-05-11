#ifndef QUEEN_H
#define QUEEN_H

#include "Piece.h"

enum class PieceColor;

class Queen : public Piece
{
public:
    Queen(PieceColor initColor);

    size_t getPriority() const noexcept override { return 15; }
    ~Queen() override = default;
protected:
    virtual bool isStrategyAcceptable(const std::unique_ptr<MovementStrategy>& strategy) const noexcept override;
};

#endif // QUEEN_H
