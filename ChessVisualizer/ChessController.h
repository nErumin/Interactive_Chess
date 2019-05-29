#ifndef CHESSCONTROLLER_H
#define CHESSCONTROLLER_H

#include "ChessGame.h"
#include "ChessWindow.h"
#include "Observer.h"

class ChessController : public Observer<Player&, Player&>,
                        public Observer<const Cell&, Vector2>
{
public:
    ChessController();
    void startChess();
    void notify(Player& changingPlayer, Player& nextPlayer) override;
    void notify(const Cell& changedCell, Vector2&& location) override;
    void finalize();

    ~ChessController() override;
private:
    void makeMovingFromPlayer(Player& player);
    void startTurn();
    void doTurn();
    void refreshWindow();

    ChessGame game;
    ChessWindow window;
};

#endif // CHESSCONTROLLER_H
