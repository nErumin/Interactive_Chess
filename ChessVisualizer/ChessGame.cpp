#include "ChessGame.h"
#include "Player.h"
#include "Board.h"
#include "PieceColor.h"
#include "PlayerType.h"

ChessGame::ChessGame()
    : currentTurnPlayerIndex{ 0 }
{
    players.emplace_back(PlayerType::Human, PieceColor::White);
    players.emplace_back(PlayerType::Human, PieceColor::Black);

    gameBoard.registerObserver(this);
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
    return players[currentTurnPlayerIndex];
}

const Player& ChessGame::getCurrentPlayer() const noexcept
{
    return players[currentTurnPlayerIndex];
}

void ChessGame::notify([[maybe_unused]] const Cell& cell, [[maybe_unused]] Vector2&& location)
{
    currentTurnPlayerIndex = (currentTurnPlayerIndex + 1) % players.size();

    notifyToObservers(players[currentTurnPlayerIndex]);
}

ChessGame::~ChessGame()
{
    gameBoard.unregisterObserver(this);
}
