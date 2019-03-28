#include "ChessWindow.h"
#include "Board.h"
#include <memory>
#include <QDialog>
#include <mutex>

ChessWindow::ChessWindow(QWidget* parent)
    : QWidget(parent),
      labels{ boardSize, std::vector<std::shared_ptr<QLabel>>() },
      backgroudLabel{ this },
      upperLabel{ this },
      lowerLabel{ this }
{
    setFixedSize(800, 800);

    initializeLabels();
    initializeBackground();

    upperLabel.setGeometry(20, 50, 300, 30);
    lowerLabel.setGeometry(20, 750, 300, 30);
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
    backgroudLabel.setGeometry(79, 79, 642, 644);
    backgroudLabel.setPixmap(QPixmap(":resource/board81.png"));
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
