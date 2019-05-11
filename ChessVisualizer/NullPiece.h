#ifndef NULLPIECE_H
#define NULLPIECE_H

#include "Piece.h"

enum class PieceColor;

class NullPiece : public Piece
{
public:

    size_t getPriority() const noexcept override { return 0; }

    NullPiece() = default;
    ~NullPiece() override = default;
protected:
    virtual bool isStrategyAcceptable(const std::unique_ptr<MovementStrategy>& strategy) const noexcept override;
};

extern std::shared_ptr<NullPiece> sharedNullPiece;

#endif // NULLPIECE_H
