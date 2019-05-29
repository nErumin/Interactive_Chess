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

#include <Client.h>
#include <Address.h>
#include <SocketConnection.h>
#include <TransmissionService.h>
#include <SerialService.h>
#include <NetworkError.h>
#include <ErrorCode.h>

#include "StringUtility.h"
#include "InformationModal.h"
#include "King.h"
#include "Queen.h"
#include "Bishop.h"
#include "Rook.h"
#include "Knight.h"
#include "Pawn.h"
#include "NullPiece.h"
#include "BoardUtility.h"

#define WOOJINS_IP ("10.210.60.150")

namespace
{
    constexpr double initialTimerTime = 12000.0;

    Network::Client recognizerClient{ Network::Address(WOOJINS_IP, 33333) };
    std::unique_ptr<Network::SocketConnection> recognizerConnection;

    Network::Client relayClient{ Network::Address("localhost", 44444) };
    std::unique_ptr<Network::SocketConnection> relayConnection;
}

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

void shutdown()
{
    InformationModal disconnectionModal{ nullptr };
    disconnectionModal.setModalTitle("Not connected");
    disconnectionModal.setMessageText("Cannot connect to recognizer/relay server.");
    disconnectionModal.exec();

    std::this_thread::sleep_for(std::chrono::duration<size_t, std::milli>{ 3000 });
    QMetaObject::invokeMethod(QApplication::instance(), "quit", Qt::QueuedConnection);

}

ChessController::ChessController()
{
    // register turn changed event
    game.registerObserver(this);

    // register cell changed event
    auto& board = game.getBoard();
    board.registerObserver(this);
}

void ChessController::refreshWindow()
{
    // initialize image
    for (size_t i = 0; i < boardSize; ++i)
    {
        for (size_t j = 0; j < boardSize; ++j)
        {
            auto piecePtr = game.getBoard().getCell(i, j).getPiece();
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

        player->getTimer().start(initialTimerTime);
        player->getTimer().pause();
    }

    try
    {
        recognizerConnection = std::make_unique<Network::SocketConnection>(recognizerClient.connect());
        relayConnection = std::make_unique<Network::SocketConnection>(relayClient.connect());

        std::cout << "Press 'ENTER' to start chess..." << std::endl;

        while (true)
        {
            std::string dummyLine;
            std::getline(std::cin, dummyLine);

            Network::TransmissionService recognizerService{ *recognizerConnection };
            recognizerService.send("0");
            auto response = recognizerService.receive(128);

            if (response == "WHITE" || response == "BLACK")
            {
                auto robotColor = response == "WHITE" ?
                            PieceColor::White :
                            PieceColor::Black;

                game.initializeGame({ getEnemyColor(robotColor), robotColor });
                refreshWindow();

                startTurn();
                break;
            }
            else
            {
                std::cout << "Cannot get valid response... Press 'ENTER' when you're ready..." << std::endl;
                std::cout << response << std::endl;
            }
        }
    }
    catch (const std::exception& error)
    {
        std::cout << error.what() << std::endl;
        shutdown();

        return;
    }
}

void ChessController::startTurn()
{
    std::thread([this]
    {
        std::cout << "Initialized... Please wait..." << std::endl;
        std::this_thread::sleep_for(std::chrono::duration<size_t, std::milli>{ 3000 });

        game.getCurrentPlayer().getTimer().resume();

        QMetaObject::invokeMethod(QApplication::instance(), [this]
        {
            doTurn();
        }, Qt::QueuedConnection);
    }).detach();
}

void ChessController::doTurn()
{
    Network::TransmissionService recognizerService{ *recognizerConnection };
    Network::TransmissionService relayService{ *relayConnection };

    if (game.getCurrentPlayer().getType() == PlayerType::Human)
    {
        std::cout << "Press 'ENTER' to finish your turn..." << std::endl;

        std::string dummyLine;
        std::getline(std::cin, dummyLine);

        try
        {
            recognizerService.send("0");
            auto differenceMessage = recognizerService.receive(1024);

            std::cout << "[Recognition] " << differenceMessage << std::endl;

            auto differences = parseDifferenceProtocolMessage(differenceMessage);

            for (auto& difference : differences)
            {
                game.movePiece(difference.first, difference.second - difference.first);
            }
        }
        catch (const std::exception& error)
        {
            std::cout << error.what() << std::endl;
            shutdown();
        }
    }
    else
    {
        bool isPassive = pickRandomNumber(0, 10) < 5;
        auto picked = randomPickPieceMoving(game.getBoard(), game.getCurrentPlayer().getOwningPieceColor(), isPassive);

        try
        {
            relayService.send(formatLocationsToProtocol({ std::make_pair(picked.first, picked.first + picked.second) }));
            auto completionMessage = relayService.receive(1024);

            game.movePiece(picked.first, picked.second);

            recognizerService.send("0");
            auto okMessage = recognizerService.receive(1024);
        }
        catch (const std::exception& error)
        {
            std::cout << error.what() << std::endl;
            shutdown();
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

    if (result == GameResult::Draw)
    {
        resultMessage[0] = static_cast<char>(toupper(resultMessage[0]));
        stream << resultMessage << "!";
    }
    else
    {
        stream << "You " << resultMessage << "!";
    }

    try
    {
        Network::TransmissionService recognizerService{ *recognizerConnection };
        recognizerService.send("-1");
    }
    catch (const std::exception& error)
    {
        std::cout << error.what() << std::endl;
        shutdown();
    }

    dialog.setModalTitle("Game Finished");
    dialog.setMessageText(stream.str());
    dialog.exec();

    std::this_thread::sleep_for(std::chrono::duration<size_t, std::milli>{ 3000 });
    QMetaObject::invokeMethod(QApplication::instance(), "quit", Qt::QueuedConnection);
}

void ChessController::finalize()
{
    game.getCurrentPlayer().getTimer().stop();
    game.getNextPlayer().getTimer().stop();

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
        finalize();

        showGameFinishedDialog(window, game.getGameResult());
        game.getBoard().unregisterObserver(this);

        return;
    }

    nextPlayer.getTimer().resume();

    startTurn();
}

void ChessController::notify(const Cell& changedCell, Vector2&& location)
{
    if (game.getGameResult() != GameResult::None)
    {
        finalize();

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
