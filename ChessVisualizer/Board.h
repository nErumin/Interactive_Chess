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

    std::vector<Vector2> findPieceMovableLocations(const Vector2 pieceLocation) const;
    void movePiece(const Vector2 pieceLocation, const Vector2 deltaLocation);
private:
    void initializeBoardCellColors();
    void initializeBoardCellPieces();
    std::vector<std::vector<PieceColor>> makeObstacleMap() const;

private:
    void PawnsFor(std::function<void (Pawn&, std::pair<size_t,size_t>)> handler) const;
    const Cell& getCell(const Vector2& location) const;
    Cell& getCell(const Vector2& location);

    std::vector<std::vector<Cell>> boardCells;
};

#endif // BOARD_H
