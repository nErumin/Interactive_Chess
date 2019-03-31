#ifndef CHESSGAME_H
#define CHESSGAME_H

#include <vector>
#include "Board.h"
#include "Player.h"
#include "Observer.h"
#include "Observable.h"
#include "PieceColor.h"

#include <memory>

class Cell;
struct Vector2;

class ChessGame : public Observer<const Cell&, Vector2>,
                  public Observable<Player&, Player&>
{
public:
    ChessGame();
    Board& getBoard() noexcept;
    const Board& getBoard() const noexcept;
    Player& getCurrentPlayer() noexcept;
    const Player& getCurrentPlayer() const noexcept;
    std::vector<std::shared_ptr<Player>> getPlayers() const;

    void movePiece(const Vector2 pieceLocation, const Vector2 deltaLocation);

    void notify(const Cell& cell, Vector2&& location) override;
    ~ChessGame() override;
private:
    std::vector<std::shared_ptr<Player>> players;
    size_t currentTurnPlayerIndex;
    Board gameBoard;
};


#endif // CHESSGAME_H
