#include <memory>
#include <exception>
#include "Vector2.h"
#include "Board.h"
#include "King.h"
#include "Queen.h"
#include "Rook.h"
#include "Bishop.h"
#include "Knight.h"
#include "Pawn.h"
#include "NullPiece.h"
#include "Cell.h"
#include "PieceColor.h"
#include <algorithm>

Board::Board()
    : boardCells{ boardSize, std::vector<Cell>(boardSize, Cell(sharedNullPiece)) }
{
    initializeBoardCellColors();
    initializeBoardCellPieces();
}

void Board::initializeBoardCellColors()
{
    for (size_t i = 0; i < boardCells.size(); ++i)
    {
        for (size_t j = 0; j < boardCells[i].size(); ++j)
        {
            auto cellColor = (i * boardCells.size() + j) % 2 == 0 ?
                             CellColor::White :
                             CellColor::Black;

            boardCells[i][j].setColor(cellColor);
        }
    }
}

void Board::initializeBoardCellPieces()
{
    boardCells[0][0].setPiece(std::make_shared<Rook>(PieceColor::Black));
    boardCells[0][1].setPiece(std::make_shared<Knight>(PieceColor::Black));
    boardCells[0][2].setPiece(std::make_shared<Bishop>(PieceColor::Black));
    boardCells[0][3].setPiece(std::make_shared<Queen>(PieceColor::Black));
    boardCells[0][4].setPiece(std::make_shared<King>(PieceColor::Black));
    boardCells[0][5].setPiece(std::make_shared<Bishop>(PieceColor::Black));
    boardCells[0][6].setPiece(std::make_shared<Knight>(PieceColor::Black));
    boardCells[0][7].setPiece(std::make_shared<Rook>(PieceColor::Black));

    boardCells[1][0].setPiece(std::make_shared<Pawn>(PieceColor::Black));
    boardCells[1][1].setPiece(std::make_shared<Pawn>(PieceColor::Black));
    boardCells[1][2].setPiece(std::make_shared<Pawn>(PieceColor::Black));
    boardCells[1][3].setPiece(std::make_shared<Pawn>(PieceColor::Black));
    boardCells[1][4].setPiece(std::make_shared<Pawn>(PieceColor::Black));
    boardCells[1][5].setPiece(std::make_shared<Pawn>(PieceColor::Black));
    boardCells[1][6].setPiece(std::make_shared<Pawn>(PieceColor::Black));
    boardCells[1][7].setPiece(std::make_shared<Pawn>(PieceColor::Black));

    boardCells[6][0].setPiece(std::make_shared<Pawn>(PieceColor::White));
    boardCells[6][1].setPiece(std::make_shared<Pawn>(PieceColor::White));
    boardCells[6][2].setPiece(std::make_shared<Pawn>(PieceColor::White));
    boardCells[6][3].setPiece(std::make_shared<Pawn>(PieceColor::White));
    boardCells[6][4].setPiece(std::make_shared<Pawn>(PieceColor::White));
    boardCells[6][5].setPiece(std::make_shared<Pawn>(PieceColor::White));
    boardCells[6][6].setPiece(std::make_shared<Pawn>(PieceColor::White));
    boardCells[6][7].setPiece(std::make_shared<Pawn>(PieceColor::White));

    std::dynamic_pointer_cast<Pawn>(boardCells[6][0].getPiece())->setRotationDegree(180.0);
    std::dynamic_pointer_cast<Pawn>(boardCells[6][1].getPiece())->setRotationDegree(180.0);
    std::dynamic_pointer_cast<Pawn>(boardCells[6][2].getPiece())->setRotationDegree(180.0);
    std::dynamic_pointer_cast<Pawn>(boardCells[6][3].getPiece())->setRotationDegree(180.0);
    std::dynamic_pointer_cast<Pawn>(boardCells[6][4].getPiece())->setRotationDegree(180.0);
    std::dynamic_pointer_cast<Pawn>(boardCells[6][5].getPiece())->setRotationDegree(180.0);
    std::dynamic_pointer_cast<Pawn>(boardCells[6][6].getPiece())->setRotationDegree(180.0);
    std::dynamic_pointer_cast<Pawn>(boardCells[6][7].getPiece())->setRotationDegree(180.0);

    boardCells[7][0].setPiece(std::make_shared<Rook>(PieceColor::White));
    boardCells[7][1].setPiece(std::make_shared<Knight>(PieceColor::White));
    boardCells[7][2].setPiece(std::make_shared<Bishop>(PieceColor::White));
    boardCells[7][3].setPiece(std::make_shared<Queen>(PieceColor::White));
    boardCells[7][4].setPiece(std::make_shared<King>(PieceColor::White));
    boardCells[7][5].setPiece(std::make_shared<Bishop>(PieceColor::White));
    boardCells[7][6].setPiece(std::make_shared<Knight>(PieceColor::White));
    boardCells[7][7].setPiece(std::make_shared<Rook>(PieceColor::White));
}

Cell& Board::getCell(const Vector2& location)
{
    auto& cellReference = const_cast<const Board*>(this)->getCell(location);
    return const_cast<Cell&>(cellReference);
}

const Cell& Board::getCell(const Vector2& location) const
{
    const auto integerVector = normalizeToIntegerVector(location);

    // (x, y) ==> (y, x) in the vector.
    return boardCells[integerVector.second][integerVector.first];
}

std::vector<std::vector<PieceColor>> Board::makeObstacleMap() const
{
    std::vector<std::vector<PieceColor>> obstacleMap{ boardCells.front().size(), std::vector<PieceColor>(boardCells.size(), PieceColor::None) };

    for (size_t i = 0; i < boardCells.size(); ++i)
    {
        for (size_t j = 0; j < boardCells[i].size(); ++j)
        {
            obstacleMap[j][i] = boardCells[i][j].getPiece()->getColor();
        }
    }

    return obstacleMap;
}

std::vector<Vector2> Board::findPieceMovableLocations(const Vector2 pieceLocation) const
{
    const auto& cell = getCell(pieceLocation);

    if (!cell.isPieceOnBoard())
    {
        return {};
    }

    auto movableLocations = cell.getPiece()->movableLocationsUsingObstacles(pieceLocation, makeObstacleMap());

    auto boundaryIterator = std::remove_if(movableLocations.begin(), movableLocations.end(), [this, &cell](const Vector2& location)
    {
        if (location.x() < 0 ||
            location.y() < 0 ||
            location.x() >= boardSize ||
            location.y() >= boardSize)
        {
            return true;
        }

        const auto& checkedCell = getCell(location);
        return checkedCell.isPieceOnBoard() &&
               checkedCell.getColor() == cell.getColor();
    });

    movableLocations.erase(boundaryIterator, movableLocations.end());

    return movableLocations;
}

void Board::movePiece(const Vector2 pieceLocation, const Vector2 deltaLocation)
{
    auto& currentCell = getCell(pieceLocation);
    auto movableLocations = findPieceMovableLocations(pieceLocation);
    auto nextLocation = pieceLocation + deltaLocation;

    if (std::find(movableLocations.cbegin(), movableLocations.cend(), nextLocation) == movableLocations.cend())
    {
        throw std::invalid_argument{ "cannot move to that location " };
    }

    auto& targetCell = getCell(nextLocation);

    // decide its movement will remove the opponent's piece.
    auto nextCurrentCellPiecePtr = targetCell.getPiece();

    if (targetCell.getPiece()->getColor() != PieceColor::None)
    {
        nextCurrentCellPiecePtr = sharedNullPiece;
    }

    targetCell.setPiece(currentCell.getPiece());
    currentCell.setPiece(nextCurrentCellPiecePtr);

    // must be checked when a pawn is moved.
    if (auto pawnPiece = std::dynamic_pointer_cast<Pawn>(targetCell.getPiece()); pawnPiece != nullptr)
    {
        pawnPiece->setPawnMoved(true);
    }

    // is there an enemy on the diagonal cells?
    PawnsFor([this](Pawn& pawn, std::pair<size_t,size_t> indices)
    {
        pawn.markDiagnoalMovable(PawnDiagonalMask::None);

        auto diagonalLocations = pawn.getDiagonalLocations({ static_cast<double>(indices.second), static_cast<double>(indices.first) });

        auto boundaryIterator = std::remove_if(diagonalLocations.begin(), diagonalLocations.end(), [this, &pawn](const auto& maskLocation)
        {
            if (maskLocation.second.x() < 0 ||
                maskLocation.second.y() < 0 ||
                maskLocation.second.x() >= boardSize ||
                maskLocation.second.y() >= boardSize)
            {
                return true;
            }

           const auto& targetCell = getCell(maskLocation.second);
           return targetCell.getPiece()->getColor() == PieceColor::None ||
                  targetCell.getPiece()->getColor() == pawn.getColor();
        });

        diagonalLocations.erase(boundaryIterator, diagonalLocations.end());

        for (const auto& maskLocation : diagonalLocations)
        {
            pawn.markDiagnoalMovable(pawn.getMask() | maskLocation.first);
        }
    });

    Vector2 previousLocation = pieceLocation;
    notifyToObservers(currentCell, std::move(previousLocation));
    notifyToObservers(targetCell, std::move(nextLocation));
}

void Board::PawnsFor(std::function<void (Pawn&, std::pair<size_t,size_t>)> handler) const
{
    for (size_t i = 0; i < boardCells.size(); ++i)
    {
        for (size_t j = 0; j < boardCells[i].size(); ++j)
        {
            if (auto pawnPiece = std::dynamic_pointer_cast<Pawn>(boardCells[i][j].getPiece()); pawnPiece != nullptr)
            {
                handler(*pawnPiece, std::make_pair(i, j));
            }
        }
    }
}

Cell& Board::getCell(size_t row, size_t column) noexcept
{
    return boardCells[row][column];
}

const Cell& Board::getCell(size_t row, size_t column) const noexcept
{
    return boardCells[row][column];
}
