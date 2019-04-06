#include "ChessGame.h"
#include "Player.h"
#include "Board.h"
#include "PieceColor.h"
#include "PlayerType.h"
#include "NullPiece.h"
#include <memory>
#include "Vector2.h"
#include "MathUtils.h"

ChessGame::ChessGame()
    : currentTurnPlayerIndex{ pickRandomNumber<size_t>(0, 2) }
{
    auto randomColors = pickRandomColorPair();

    players.push_back(std::make_shared<Player>(PlayerType::Human, randomColors.first));
    players.push_back(std::make_shared<Player>(PlayerType::Robot, randomColors.second));

    gameBoard.initializeBoardCellPieces(randomColors.second, randomColors.first);
}

std::vector<std::shared_ptr<Player>> ChessGame::getPlayers() const
{
    std::vector<std::shared_ptr<Player>> returnPlayers(players.size());

    for (size_t i = 0; i < players.size(); ++i)
    {
        returnPlayers[i] = players[i];
    }

    return returnPlayers;
}

Board& ChessGame::getBoard() noexcept
{
    return gameBoard;
}

const Board& ChessGame::getBoard() const noexcept
{
    return gameBoard;
}

Player& ChessGame::getCurrentPlayer() noexcept
{
    return *players[currentTurnPlayerIndex];
}

const Player& ChessGame::getCurrentPlayer() const noexcept
{
    return *players[currentTurnPlayerIndex];
}


void ChessGame::movePiece(const Vector2 pieceLocation, const Vector2 deltaLocation)
{
    auto indices = normalizeToIntegerVector(pieceLocation);
    auto selectedPieceColor = getBoard().getCell(indices.second, indices.first).getPiece()->getColor();

    if (selectedPieceColor != getCurrentPlayer().getOwningPieceColor())
    {
        throw std::logic_error{ "not your piece" };
    }

    getBoard().movePiece(pieceLocation, deltaLocation);
}

void ChessGame::setToNextPlayer()
{
    Player& changingPlayer = *players[currentTurnPlayerIndex];
    currentTurnPlayerIndex = (currentTurnPlayerIndex + 1) % players.size();

    notifyToObservers(changingPlayer, *players[currentTurnPlayerIndex]);
}

ChessGame::~ChessGame()
{
}
