#ifndef CHESSGAME_H
#define CHESSGAME_H

#include <vector>
#include "Board.h"
#include "Player.h"
#include "Observer.h"
#include "Observable.h"

class Cell;
struct Vector2;

class ChessGame : public Observer<const Cell&, Vector2>,
                  public Observable<Player&>
{
public:
    ChessGame();
    Board& getBoard() noexcept;
    const Board& getBoard() const noexcept;
    Player& getCurrentPlayer() noexcept;
    const Player& getCurrentPlayer() const noexcept;

    void notify(const Cell& cell, Vector2&& location) override;

    ~ChessGame() override;
private:
    std::vector<Player> players;
    size_t currentTurnPlayerIndex;
    Board gameBoard;
};


#endif // CHESSGAME_H