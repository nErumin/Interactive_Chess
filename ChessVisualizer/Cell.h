#ifndef CELL_H
#define CELL_H

#include <memory>
#include "CellColor.h"

class Piece;

class Cell
{
public:
    explicit Cell(std::shared_ptr<Piece> cellPiece, CellColor initColor = CellColor::None);

    CellColor getCellColor() const noexcept;
    bool isPieceOnBoard() const;

    std::shared_ptr<Piece> getPiece() const;
    void setPiece(std::shared_ptr<Piece> newPiece);
private:
    std::shared_ptr<Piece> piece;
    CellColor color;
};

#endif // CELL_H
