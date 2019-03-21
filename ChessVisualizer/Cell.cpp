#include "Cell.h"
#include "CellColor.h"
#include "Piece.h"
#include "NullPiece.h"
#include <exception>

Cell::Cell(std::shared_ptr<Piece> cellPiece, CellColor initColor)
    : piece{ cellPiece },
      color{ initColor }
{
}

CellColor Cell::getCellColor() const noexcept
{
    return color;
}

bool Cell::isPieceOnBoard() const
{
    return std::dynamic_pointer_cast<NullPiece>(piece) == nullptr;
}

std::shared_ptr<Piece> Cell::getPiece() const
{
    return piece;
}

void Cell::setPiece(std::shared_ptr<Piece> newPiece)
{
    if (piece->getColor() == newPiece->getColor())
    {
        throw std::invalid_argument{ "cannot place same color piece on this cell" };
    }

    piece = newPiece;
}
