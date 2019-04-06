#include "ChessWindow.h"
#include "ChessController.h"
#include "Board.h"
#include "Vector2.h"
#include "PlayerType.h"
#include <QPixmap>
#include <exception>
#include <memory>
#include <iostream>
#include <sstream>

#include "InformationModal.h"
#include "King.h"
#include "Queen.h"
#include "Bishop.h"
#include "Rook.h"
#include "Knight.h"
#include "Pawn.h"

QPixmap convertPieceToPixmap(const Piece* piecePtr)
{
    std::string pixmapName;
    pixmapName.assign(piecePtr->getColor() == PieceColor::White ? "_white.svg" : "_black.svg");

    if (dynamic_cast<const King*>(piecePtr) != nullptr)
    {
        pixmapName.insert(0, ":resource/king");
        return QPixmap{ pixmapName.c_str() };
    }
    else if (dynamic_cast<const Queen*>(piecePtr) != nullptr)
    {
        pixmapName.insert(0, ":resource/queen");
        return QPixmap{ pixmapName.c_str() };
    }
    else if (dynamic_cast<const Bishop*>(piecePtr) != nullptr)
    {
        pixmapName.insert(0, ":resource/bishop");
        return QPixmap{ pixmapName.c_str() };
    }
    else if (dynamic_cast<const Rook*>(piecePtr) != nullptr)
    {
        pixmapName.insert(0, ":resource/rook");
        return QPixmap{ pixmapName.c_str() };
    }
    else if (dynamic_cast<const Knight*>(piecePtr) != nullptr)
    {
        pixmapName.insert(0, ":resource/knight");
        return QPixmap{ pixmapName.c_str() };
    }
    else if (dynamic_cast<const Pawn*>(piecePtr) != nullptr)
    {
        pixmapName.insert(0, ":resource/pawn");
        return QPixmap{ pixmapName.c_str() };
    }
    else
    {
        return QPixmap();
    }
}

ChessController::ChessController()
{
    // register turn changed event
    game.registerObserver(this);

    // register cell changed event
    auto& board = game.getBoard();
    board.registerObserver(this);

    // initialize image
    for (size_t i = 0; i < boardSize; ++i)
    {
        for (size_t j = 0; j < boardSize; ++j)
        {
            auto piecePtr = board.getCell(i, j).getPiece();
            window.getLabel(i, j).setPixmap(convertPieceToPixmap(piecePtr.get()));
        }
    }
}

void ChessController::startChess()
{
    window.show();

    // set timers and register tick events
    for (const auto& player : game.getPlayers())
    {
        auto& label = player->getOwningPieceColor() == PieceColor::Black ?
                    window.getUpperLabel() :
                    window.getLowerLabel();

        label.setText("Remain Time: 10");

        player->getTimer().setTickHandler([&label](double leftTime)
        {
            double difference = leftTime - std::floor(leftTime);
            if (difference > 0 && difference < 0.001)
            {
                std::ostringstream stringStream;
                stringStream << "Remain Time: " << static_cast<size_t>(std::floor(leftTime)) << std::endl;

                label.setText(stringStream.str().c_str());
            }
        });

        player->getTimer().setFinishedHandler([]
        {
            std::cout << "Timer Finished!" << std::endl;
        });

        player->getTimer().start(10.0);
        player->getTimer().pause();
    }

   game.getCurrentPlayer().getTimer().resume();
   makeMovingFromPlayer(game.getCurrentPlayer());
}


void ChessController::makeMovingFromPlayer(Player& player)
{
    if (player.getType() == PlayerType::Human)
    {
        std::cout << "Select!" << std::endl;
        while (true)
        {
            double x = 0;
            double y = 1;
            double deltaX = 0;
            double deltaY = 3;

            std::cin >> x >> y >> deltaX >> deltaY;

            try
            {
                game.movePiece({ x, y }, { deltaX, deltaY });
            }
            catch (const std::exception&)
            {
                auto& currentPlayerTimer = game.getCurrentPlayer().getTimer();

                currentPlayerTimer.pause();

                InformationModal modal{ nullptr };

                modal.setModalTitle("Wrong placing");
                modal.setMessageText("You cannot make that movement. It's wrong!");
                modal.exec();

                currentPlayerTimer.resume();

                continue;
            }

            break;
        }
    }
}

void ChessController::notify(Player& changingPlayer, Player& nextPlayer)
{
    changingPlayer.getTimer().pause();

    if (game.getBoard().isColorChecked(changingPlayer.getOwningPieceColor()))
    {
        std::string message = changingPlayer.getOwningPieceColor() == PieceColor::Black ?
                    "The black king" : "The white king";

        InformationModal modal{ nullptr };
        modal.setModalTitle("Check!");
        modal.setMessageText(message + " is checked!");
        modal.exec();
    }

    if (game.getBoard().isColorChecked(nextPlayer.getOwningPieceColor()))
    {
        std::string message = nextPlayer.getOwningPieceColor() == PieceColor::Black ?
                    "The black king" : "The white king";

        InformationModal modal{ nullptr };
        modal.setModalTitle("Check!");
        modal.setMessageText(message + " is checked!");
        modal.exec();
    }

    nextPlayer.getTimer().resume();
}

void ChessController::notify(const Cell& changedCell, Vector2&& location)
{
    auto pixmap = convertPieceToPixmap(changedCell.getPiece().get());
    auto normalized = normalizeToIntegerVector(location);

    window.getLabel(normalized.second, normalized.first).setPixmap(std::move(pixmap));
}

ChessController::~ChessController()
{
    game.unregisterObserver(this);
}
