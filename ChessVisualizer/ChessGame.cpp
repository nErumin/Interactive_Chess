#include "ChessGame.h"
#include "Player.h"
#include "Board.h"
#include "PieceColor.h"
#include "PlayerType.h"
#include "NullPiece.h"
#include <memory>

ChessGame::ChessGame()
    : currentTurnPlayerIndex{ 0 }
{
    players.push_back(std::make_shared<Player>(PlayerType::Human, PieceColor::White));
    players.push_back(std::make_shared<Player>(PlayerType::Human, PieceColor::Black));

    gameBoard.registerObserver(this);
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

void ChessGame::notify([[maybe_unused]] const Cell& cell, [[maybe_unused]] Vector2&& location)
{
    if (std::dynamic_pointer_cast<NullPiece>(cell.getPiece()) != nullptr)
    {
        Player& changingPlayer = *players[currentTurnPlayerIndex];
        currentTurnPlayerIndex = (currentTurnPlayerIndex + 1) % players.size();

        notifyToObservers(changingPlayer, *players[currentTurnPlayerIndex]);
    }
}

ChessGame::~ChessGame()
{
    gameBoard.unregisterObserver(this);
}
