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
#include <future>
#include <chrono>

#include <QApplication>
#include <cstdlib>
#include "InformationModal.h"
#include "King.h"
#include "Queen.h"
#include "Bishop.h"
#include "Rook.h"
#include "Knight.h"
#include "Pawn.h"
#include "NullPiece.h"

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
        auto& label = player->getOwningPieceColor() == game.getBoard().getTopPieceColor() ?
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

        player->getTimer().setFinishedHandler([playerType = player->getType(), this]
        {
            auto result = playerType == PlayerType::Human ?
                        GameResult::Lose :
                        GameResult::Win;

            std::cout << "Timer Finished! Result = " << toLowerString(result) << std::endl;

            game.setGameResult(result);
        });

        player->getTimer().start(5.0);
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

inline void showDialogIfChecked(const Board& board, PieceColor pieceColor)
{
    if (board.isColorChecked(pieceColor))
    {
        std::string message = pieceColor == PieceColor::Black ?
                    "The black king" : "The white king";

        InformationModal modal{ nullptr };
        modal.setModalTitle("Check!");
        modal.setMessageText(message + " is checked!");
        modal.exec();
    }
}

void ChessController::notify(Player& changingPlayer, Player& nextPlayer)
{
    changingPlayer.getTimer().pause();

    showDialogIfChecked(game.getBoard(), changingPlayer.getOwningPieceColor());
    showDialogIfChecked(game.getBoard(), nextPlayer.getOwningPieceColor());

    nextPlayer.getTimer().resume();
}

inline void showGameFinishedDialog(ChessWindow& window, GameResult result)
{
    window.setWindowColor({ 0, 0, 0, 75 });

    InformationModal dialog{ nullptr };
    std::ostringstream stream;

    auto resultMessage = toLowerString(result);

    if (result == GameResult::Draw)
    {
        resultMessage[0] = static_cast<char>(toupper(resultMessage[0]));
        stream << resultMessage << "!";
    }
    else
    {
        stream << "You " << resultMessage << "!";
    }

    dialog.setModalTitle("Game Finished");
    dialog.setMessageText(stream.str());
    dialog.exec();

    QMetaObject::invokeMethod(QApplication::instance(), "quit", Qt::QueuedConnection);
}

void ChessController::notify(const Cell& changedCell, Vector2&& location)
{
    if (game.getGameResult() != GameResult::None)
    {
        showGameFinishedDialog(window, game.getGameResult());
        game.getBoard().unregisterObserver(this);

        return;
    }

    auto pixmap = convertPieceToPixmap(changedCell.getPiece().get());
    auto normalized = normalizeToIntegerVector(location);

    window.getLabel(normalized.second, normalized.first).setPixmap(std::move(pixmap));

    if (std::dynamic_pointer_cast<NullPiece>(changedCell.getPiece()) == nullptr)
    {
        game.setToNextPlayer();
    }
}

ChessController::~ChessController()
{
    game.unregisterObserver(this);
}
