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
#include <cmath>
#include "Piece.h"
#include "MathUtils.h"
#include <map>

Board::Board()
    : boardCells{ boardSize, std::vector<Cell>(boardSize, Cell(sharedNullPiece)) }
{
    initializeBoardCellColors();
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

void Board::initializeBoardCellPieces(PieceColor topPieceColor, PieceColor bottomPieceColor)
{
    pieceColors.first = topPieceColor;
    pieceColors.second = bottomPieceColor;

    boardCells[0][0].setPiece(std::make_shared<Rook>(pieceColors.first));
    boardCells[0][1].setPiece(std::make_shared<Knight>(pieceColors.first));
    boardCells[0][2].setPiece(std::make_shared<Bishop>(pieceColors.first));
    boardCells[0][3].setPiece(std::make_shared<Queen>(pieceColors.first));
    boardCells[0][4].setPiece(std::make_shared<King>(pieceColors.first));
    boardCells[0][5].setPiece(std::make_shared<Bishop>(pieceColors.first));
    boardCells[0][6].setPiece(std::make_shared<Knight>(pieceColors.first));
    boardCells[0][7].setPiece(std::make_shared<Rook>(pieceColors.first));

    boardCells[1][0].setPiece(std::make_shared<Pawn>(pieceColors.first));
    boardCells[1][1].setPiece(std::make_shared<Pawn>(pieceColors.first));
    boardCells[1][2].setPiece(std::make_shared<Pawn>(pieceColors.first));
    boardCells[1][3].setPiece(std::make_shared<Pawn>(pieceColors.first));
    boardCells[1][4].setPiece(std::make_shared<Pawn>(pieceColors.first));
    boardCells[1][5].setPiece(std::make_shared<Pawn>(pieceColors.first));
    boardCells[1][6].setPiece(std::make_shared<Pawn>(pieceColors.first));
    boardCells[1][7].setPiece(std::make_shared<Pawn>(pieceColors.first));

    boardCells[6][0].setPiece(std::make_shared<Pawn>(pieceColors.second));
    boardCells[6][1].setPiece(std::make_shared<Pawn>(pieceColors.second));
    boardCells[6][2].setPiece(std::make_shared<Pawn>(pieceColors.second));
    boardCells[6][3].setPiece(std::make_shared<Pawn>(pieceColors.second));
    boardCells[6][4].setPiece(std::make_shared<Pawn>(pieceColors.second));
    boardCells[6][5].setPiece(std::make_shared<Pawn>(pieceColors.second));
    boardCells[6][6].setPiece(std::make_shared<Pawn>(pieceColors.second));
    boardCells[6][7].setPiece(std::make_shared<Pawn>(pieceColors.second));

    std::dynamic_pointer_cast<Pawn>(boardCells[6][0].getPiece())->setRotationDegree(180.0);
    std::dynamic_pointer_cast<Pawn>(boardCells[6][1].getPiece())->setRotationDegree(180.0);
    std::dynamic_pointer_cast<Pawn>(boardCells[6][2].getPiece())->setRotationDegree(180.0);
    std::dynamic_pointer_cast<Pawn>(boardCells[6][3].getPiece())->setRotationDegree(180.0);
    std::dynamic_pointer_cast<Pawn>(boardCells[6][4].getPiece())->setRotationDegree(180.0);
    std::dynamic_pointer_cast<Pawn>(boardCells[6][5].getPiece())->setRotationDegree(180.0);
    std::dynamic_pointer_cast<Pawn>(boardCells[6][6].getPiece())->setRotationDegree(180.0);
    std::dynamic_pointer_cast<Pawn>(boardCells[6][7].getPiece())->setRotationDegree(180.0);

    boardCells[7][0].setPiece(std::make_shared<Rook>(pieceColors.second));
    boardCells[7][1].setPiece(std::make_shared<Knight>(pieceColors.second));
    boardCells[7][2].setPiece(std::make_shared<Bishop>(pieceColors.second));
    boardCells[7][3].setPiece(std::make_shared<Queen>(pieceColors.second));
    boardCells[7][4].setPiece(std::make_shared<King>(pieceColors.second));
    boardCells[7][5].setPiece(std::make_shared<Bishop>(pieceColors.second));
    boardCells[7][6].setPiece(std::make_shared<Knight>(pieceColors.second));
    boardCells[7][7].setPiece(std::make_shared<Rook>(pieceColors.second));
}

PieceColor Board::getTopPieceColor() const noexcept
{
    return pieceColors.first;
}

PieceColor Board::getBottomPieceColor() const noexcept
{
    return pieceColors.second;
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

std::vector<Vector2> Board::findPieceMovableLocations(const Vector2 pieceLocation, PieceColor colorThreshold) const
{
    const auto& cell = getCell(pieceLocation);

    if (!cell.isPieceOnBoard())
    {
        return {};
    }

    auto movableLocations = cell.getPiece()->movableLocationsUsingObstacles(pieceLocation, makeObstacleMap(), colorThreshold);

    auto boundaryIterator = std::remove_if(movableLocations.begin(), movableLocations.end(), [](const Vector2& location)
    {
        if (std::round(location.x()) < 0 ||
            std::round(location.y()) < 0 ||
            std::round(location.x()) >= boardSize ||
            std::round(location.y()) >= boardSize)
        {
            return true;
        }

        return false;
    });

    movableLocations.erase(boundaryIterator, movableLocations.end());

    return movableLocations;
}

void Board::movePiece(const Vector2 pieceLocation, const Vector2 deltaLocation)
{
    auto& currentCell = getCell(pieceLocation);
    auto movableLocations = findPieceMovableLocations(pieceLocation, currentCell.getPiece()->getColor());
    auto nextLocation = pieceLocation + deltaLocation;

    auto searchResult = std::find_if(movableLocations.cbegin(), movableLocations.cend(), [nextLocation](const Vector2& movableLocation)
    {
        return normalizeToIntegerVector(movableLocation) == normalizeToIntegerVector(nextLocation);
    });

    auto& targetCell = getCell(nextLocation);

    // decide its movement will remove the opponent's piece.
    auto nextCurrentCellPiecePtr = targetCell.getPiece();
    auto originalTargetPiece = targetCell.getPiece();

    if (targetCell.getPiece()->getColor() != PieceColor::None)
    {
        nextCurrentCellPiecePtr = sharedNullPiece;
    }

    targetCell.setPiece(currentCell.getPiece());
    currentCell.setPiece(nextCurrentCellPiecePtr);

    if (isPieceTypeOf<King>(targetCell.getPiece().get()) && isChecked(nextLocation))
    {
        currentCell.setPiece(targetCell.getPiece());
        targetCell.setPiece(originalTargetPiece);

        throw std::invalid_argument{ "cannot move to that location " };
    }

    if (searchResult == movableLocations.cend())
    {
        throw std::invalid_argument{ "cannot move to that location " };
    }

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

template <typename T>
std::vector<Vector2> Board::findPieces() const
{
    std::vector<Vector2> pieceLocations;

    for (size_t i = 0; i < boardCells.size(); ++i)
    {
        for (size_t j = 0; j < boardCells[i].size(); ++j)
        {
            if (std::dynamic_pointer_cast<T>(boardCells[i][j].getPiece()) != nullptr)
            {
                pieceLocations.push_back({ static_cast<double>(j), static_cast<double>(i) });
            }
        }
    }

    return pieceLocations;
}

bool Board::isColorChecked(PieceColor color) const
{
    Vector2 kingPieceLocation;

    for (const auto& location : findPieces<King>())
    {
        auto indices = normalizeToIntegerVector(location);
        if (boardCells[indices.second][indices.first].getPiece()->getColor() == color)
        {
            kingPieceLocation = location;
        }
    }

    return isChecked(kingPieceLocation);
}

bool Board::isPieceTracedByOther(const Vector2& targetLocation, const Vector2& pieceLocation) const
{
    const auto& targetIndices = normalizeToIntegerVector(targetLocation);
    const auto& targetPiece = boardCells[targetIndices.second][targetIndices.first].getPiece();

    return isPieceTracedByOtherSimulated(targetLocation, pieceLocation, targetPiece->getColor());
}

bool Board::isPieceTracedByOtherSimulated(const Vector2& targetLocation, const Vector2& pieceLocation, PieceColor targetColor) const
{
    const auto& tracerIndices = normalizeToIntegerVector(pieceLocation);
    const auto& tracerPiece = boardCells[tracerIndices.second][tracerIndices.first].getPiece();

    auto opponentColor = getEnemyColor(targetColor);

    if (tracerPiece->getColor() == opponentColor)
    {
        auto movableLocation = findPieceMovableLocations(pieceLocation, tracerPiece->getColor());
        auto foundResult = std::find_if(movableLocation.cbegin(), movableLocation.cend(), [&targetLocation](const Vector2& location)
        {
            return normalizeToIntegerVector(location) == normalizeToIntegerVector(targetLocation);
        });

        if (foundResult != movableLocation.cend())
        {
            return true;
        }
    }

    return false;
}

bool Board::isChecked(const Vector2& kingLocation) const
{
    for (size_t i = 0; i < boardCells.size(); ++i)
    {
        for (size_t j = 0; j < boardCells[i].size(); ++j)
        {
            if (isPieceTracedByOther(kingLocation, { static_cast<double>(j), static_cast<double>(i) }))
            {
                return true;
            }
        }
    }

    return false;
}

bool Board::isStaleMated(PieceColor pieceColor) const
{
    std::vector<Vector2> opponentPieceMovableLocations;
    std::vector<Vector2> kingMovableLocations;

    for (size_t i = 0; i < boardCells.size(); ++i)
    {
        for (size_t j = 0; j < boardCells[i].size(); ++j)
        {
            auto piece = getCell(i, j).getPiece();
            Vector2 pieceLocation{ static_cast<double>(j), static_cast<double>(i) };
            auto threshold = piece->getColor() == pieceColor ? pieceColor : PieceColor::None;
            auto movableLocations = findPieceMovableLocations(pieceLocation, threshold);
            auto* copyingTargetContainer = &opponentPieceMovableLocations;

            if (piece->getColor() == pieceColor)
            {
                if (std::dynamic_pointer_cast<King>(piece) == nullptr && movableLocations.size() > 0)
                {
                    return false;
                }

                copyingTargetContainer = &kingMovableLocations;
            }

            if (auto pawnPtr = std::dynamic_pointer_cast<Pawn>(piece);
                    piece->getColor() != pieceColor && pawnPtr != nullptr)
            {
                auto diagnoalLocations = pawnPtr->getDiagonalLocations(pieceLocation);

                movableLocations.clear();
                movableLocations.push_back(diagnoalLocations.at(0).second);
                movableLocations.push_back(diagnoalLocations.at(1).second);
            }

            std::copy(movableLocations.begin(), movableLocations.end(), std::back_inserter(*copyingTargetContainer));
        }
    }

    return std::all_of(kingMovableLocations.begin(), kingMovableLocations.end(),
                       [&opponentPieceMovableLocations](const Vector2& kingMovableLocation)
    {
        return std::find(opponentPieceMovableLocations.begin(), opponentPieceMovableLocations.end(), kingMovableLocation)
                != opponentPieceMovableLocations.end();
    });
}

bool Board::isKingDead(PieceColor color) const
{
    auto kings = findPieces<King>();

    return std::all_of(kings.cbegin(), kings.cend(), [color, this](const Vector2& kingLocation)
    {
        return getCell(kingLocation).getPiece()->getColor() != color;
    });
}
