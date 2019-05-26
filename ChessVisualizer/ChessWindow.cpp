#include "ChessWindow.h"
#include "Board.h"
#include <memory>
#include <QDialog>
#include <QIcon>
#include <mutex>
#include <sstream>

ChessWindow::ChessWindow(QWidget* parent)
    : QWidget(parent),
      labels{ boardSize, std::vector<std::shared_ptr<QLabel>>() },
      windowLabel{ this },
      chessBoardLabel{ this },
      upperLabel{ this },
      lowerLabel{ this }
{
    setFixedSize(800, 800);
    setWindowIcon(QIcon(":resource/pawn_black.svg"));
    setWindowTitle("ChessVisualizer");

    initializeLabels();
    initializeBackground();

    upperLabel.setGeometry(20, 50, 300, 30);
    lowerLabel.setGeometry(20, 750, 300, 30);
    windowLabel.setGeometry(0, 0, 800, 800);

}

void ChessWindow::initializeLabels()
{
    for (size_t i = 0; i < boardSize; ++i)
    {
        for (size_t j = 0; j < boardSize; ++j)
        {
            auto newLabel = std::make_shared<QLabel>(this);

            auto xLocation = static_cast<int>(104 + (j * 76));
            auto yLocation = static_cast<int>(111 + (i * 76));

            newLabel->setGeometry(xLocation, yLocation, 59, 51);

            labels[i].push_back(newLabel);
        }
    }
}

void ChessWindow::initializeBackground()
{
    chessBoardLabel.setGeometry(79, 79, 642, 644);
    chessBoardLabel.setPixmap(QPixmap(":resource/board81.png"));
}

QLabel& ChessWindow::getLabel(size_t row, size_t column) noexcept
{
    return *(labels[row][column]);
}

const QLabel& ChessWindow::getLabel(size_t row, size_t column) const noexcept
{
    return *(labels[row][column]);
}

QLabel& ChessWindow::getUpperLabel() noexcept
{
    std::lock_guard<std::mutex> guard{ resMutex };
    return upperLabel;
}

const QLabel& ChessWindow::getUpperLabel() const noexcept
{
    std::lock_guard<std::mutex> guard{ resMutex };
    return upperLabel;
}

QLabel& ChessWindow::getLowerLabel() noexcept
{
    std::lock_guard<std::mutex> guard{ resMutex };
    return lowerLabel;
}

const QLabel& ChessWindow::getLowerLabel() const noexcept
{
    std::lock_guard<std::mutex> guard{ resMutex };
    return lowerLabel;
}

void ChessWindow::setWindowColor(std::tuple<int, int, int, int> colorVector)
{
    std::ostringstream stream;

    stream << "background-color: rgba(" << std::get<0>(colorVector) << ", " <<
                                           std::get<1>(colorVector) << ", " <<
                                           std::get<2>(colorVector) << ", " <<
                                           std::get<3>(colorVector) << "% " << ");";

    windowLabel.setStyleSheet(stream.str().c_str());
    windowLabel.raise();
}
