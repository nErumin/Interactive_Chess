#ifndef CHESSCONTROLLER_H
#define CHESSCONTROLLER_H

#include "ChessGame.h"
#include "ChessWindow.h"
#include "Observer.h"

class ChessController : public Observer<Player&>,
                        public Observer<const Cell&, Vector2>
{
public:
    ChessController();
    void startChess();
    void notify(Player& changedPlayer) override;
    void notify(const Cell& changedCell, Vector2&& location) override;

    ~ChessController() override;
private:
    void makeMovingFromPlayer(Player& player);

    ChessGame game;
    ChessWindow window;
};

#endif // CHESSCONTROLLER_H
