#ifndef PAWN_H
#define PAWN_H

#include "Piece.h"

enum class PieceColor;

enum class PawnDiagonalMask : size_t
{
    None = 0,
    Left = 1,
    Right = 2
};

inline constexpr PawnDiagonalMask operator&(const PawnDiagonalMask& lhs, const PawnDiagonalMask& rhs)
{
    auto lhsInteger = static_cast<size_t>(lhs);
    auto rhsInteger = static_cast<size_t>(rhs);

    return static_cast<PawnDiagonalMask>(lhsInteger & rhsInteger);
}

inline constexpr PawnDiagonalMask operator|(const PawnDiagonalMask& lhs, const PawnDiagonalMask& rhs)
{
    auto lhsInteger = static_cast<size_t>(lhs);
    auto rhsInteger = static_cast<size_t>(rhs);

    return static_cast<PawnDiagonalMask>(lhsInteger | rhsInteger);
}

class Pawn : public Piece
{
public:
    Pawn(PieceColor initColor);

    PawnDiagonalMask getMask() const noexcept;

    size_t getPriority() const noexcept override { return 3; }
    void setRotationDegree(double newDegree) noexcept override;
    void setPawnMoved(bool isMoved) noexcept;
    void markDiagnoalMovable(PawnDiagonalMask mask) noexcept;

    std::vector<std::pair<PawnDiagonalMask, Vector2>> getDiagonalLocations(const Vector2& centerLocation) const;
    ~Pawn() override = default;
protected:
    virtual bool isStrategyAcceptable(const std::unique_ptr<MovementStrategy>& strategy) const noexcept override;
private:
    void reconstructStrategy();

private:
    bool isNotMovedYet;
    PawnDiagonalMask diagonalMask;
};

#endif // PAWN_H
