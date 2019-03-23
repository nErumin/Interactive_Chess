#ifndef CELL_H
#define CELL_H

#include <memory>
#include "CellColor.h"
#include "Observable.h"

class Piece;

class Cell final : public Observable<Cell&, bool>
{
public:
    explicit Cell(std::shared_ptr<Piece> cellPiece, CellColor initColor = CellColor::None);

    CellColor getColor() const noexcept;
    bool isPieceOnBoard() const;

    std::shared_ptr<Piece> getPiece() const;
    void setPiece(std::shared_ptr<Piece> newPiece);
    void setColor(CellColor newColor) noexcept;
private:
    std::shared_ptr<Piece> piece;
    CellColor color;
};

#endif // CELL_H
