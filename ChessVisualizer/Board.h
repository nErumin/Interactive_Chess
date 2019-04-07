#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <functional>
#include "Cell.h"
#include "Observable.h"

enum class PieceColor;

class Pawn;
struct Vector2;

constexpr size_t boardSize = 8;

class Board final : public Observable<const Cell&, Vector2>
{
public:
    Board();

    std::vector<Vector2> findPieceMovableLocations(const Vector2 pieceLocation, PieceColor colorThreshold) const;
    void movePiece(const Vector2 pieceLocation, const Vector2 deltaLocation);

    Cell& getCell(size_t row, size_t column) noexcept;
    const Cell& getCell(size_t row, size_t column) const noexcept;

    template <typename T>
    std::vector<Vector2> findPieces() const;

    bool isPieceTracedByOther(const Vector2& targetLocation, const Vector2& pieceLocation) const;
    bool isPieceTracedByOtherSimulated(const Vector2& targetLocation, const Vector2& pieceLocation, PieceColor targetColor) const;
    bool isColorChecked(PieceColor color) const;
    bool isChecked(const Vector2& kingLocation) const;
    bool isStaleMated(PieceColor color) const;

    void initializeBoardCellPieces(PieceColor topPieceColor, PieceColor bottomPieceColor);
    PieceColor getTopPieceColor() const noexcept;
    PieceColor getBottomPieceColor() const noexcept;
private:
    void initializeBoardCellColors();
    std::vector<std::vector<PieceColor>> makeObstacleMap() const;
    void PawnsFor(std::function<void (Pawn&, std::pair<size_t,size_t>)> handler) const;

private:
    const Cell& getCell(const Vector2& location) const;
    Cell& getCell(const Vector2& location);

    std::vector<std::vector<Cell>> boardCells;
    std::pair<PieceColor, PieceColor> pieceColors;
};

#endif // BOARD_H
