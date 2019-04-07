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
#include "BoardUtility.h"

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

        label.setText("");

        player->getTimer().setTickHandler([&label](double leftTime)
        {
            double difference = leftTime - std::floor(leftTime);
            if (difference > 0 && difference < 0.001)
            {
                std::ostringstream stringStream;
                stringStream << "Remain Time: " << static_cast<size_t>(std::floor(leftTime)) << std::endl;

                QMetaObject::invokeMethod(&label, "setText", Qt::QueuedConnection, Q_ARG(QString, stringStream.str().c_str()));
                //label.setText(stringStream.str().c_str());

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

        player->getTimer().start(50.0);
        player->getTimer().pause();
    }

   game.getCurrentPlayer().getTimer().resume();
   startTurn();
}

void ChessController::startTurn()
{
    std::this_thread::sleep_for(std::chrono::duration<size_t, std::milli>{ 1000 });

    bool isPassive = pickRandomNumber(0, 2) == 0;
    auto picked = randomPickPieceMoving(game.getBoard(), game.getCurrentPlayer().getOwningPieceColor(), isPassive);

    game.movePiece(picked.first, picked.second);
}

void ChessController::makeMovingFromPlayer(Player& player)
{
    if (player.getType() == PlayerType::Human)
    {
        std::cout << "Select!" << std::endl;

        bool isPassive = pickRandomNumber(0, 2) == 0;
        auto picked = randomPickPieceMoving(game.getBoard(), player.getOwningPieceColor(), isPassive);

        std::cout << "Picked" << std::endl;
        std::this_thread::sleep_for(std::chrono::duration<size_t, std::milli>{ 1000 });
        game.movePiece(picked.first, picked.second);
        /*
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
        */
    }
    else if (player.getType() == PlayerType::Robot)
    {
        bool isPassive = pickRandomNumber(0, 2) == 0;
        auto picked = randomPickPieceMoving(game.getBoard(), player.getOwningPieceColor(), isPassive);

        std::cout << "Picked" << std::endl;
        std::this_thread::sleep_for(std::chrono::duration<size_t, std::milli>{ 1000 });
        game.movePiece(picked.first, picked.second);
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

inline void showDialogIfStaleMated(ChessGame& game)
{
    auto& board = game.getBoard();

    if (board.isStaleMated(PieceColor::Black) ||
        board.isStaleMated(PieceColor::White))
    {
        game.setGameResult(GameResult::Draw);

        InformationModal modal{ nullptr };

        modal.setModalTitle("Stalemated");
        modal.setMessageText("Stalemated!");

        modal.exec();
    }
}

inline void showDialogIfKingDead(ChessGame& game)
{
    auto& board = game.getBoard();
    auto currentPlayerColor = game.getCurrentPlayer().getOwningPieceColor();
    auto humanColor = game.getCurrentPlayer().getType() == PlayerType::Human ?
                currentPlayerColor :
                getEnemyColor(currentPlayerColor);

    InformationModal dialog{ nullptr };
    dialog.setModalTitle("King dead!");

    if (board.isKingDead(humanColor))
    {
        game.setGameResult(GameResult::Lose);

        dialog.setMessageText("Your king is dead!");
        dialog.exec();
    }
    else if (board.isKingDead(getEnemyColor(humanColor)))
    {
        game.setGameResult(GameResult::Win);

        dialog.setMessageText("The robot's king is dead!");
        dialog.exec();
    }
}

inline void showGameFinishedDialog(ChessWindow& window, GameResult result)
{
    window.setWindowColor({ 0, 0, 0, 75 });

    InformationModal dialog{ nullptr };
    std::ostringstream stream;

    auto resultMessage = toLowerString(result);

    QApplication::postEvent(QApplication::instance(), nullptr);
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

    std::this_thread::sleep_for(std::chrono::duration<size_t, std::milli>{ 1500 });
    QMetaObject::invokeMethod(QApplication::instance(), "quit", Qt::QueuedConnection);
}

void ChessController::notify(Player& changingPlayer, Player& nextPlayer)
{
    changingPlayer.getTimer().pause();

    showDialogIfChecked(game.getBoard(), changingPlayer.getOwningPieceColor());
    showDialogIfChecked(game.getBoard(), nextPlayer.getOwningPieceColor());
    showDialogIfStaleMated(game);
    showDialogIfKingDead(game);

    if (game.getGameResult() != GameResult::None)
    {
        changingPlayer.getTimer().stop();
        nextPlayer.getTimer().stop();

        showGameFinishedDialog(window, game.getGameResult());
        game.getBoard().unregisterObserver(this);

        return;
    }

    nextPlayer.getTimer().resume();
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
