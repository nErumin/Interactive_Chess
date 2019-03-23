#include "ChessWindow.h"
#include "Board.h"
#include <memory>

ChessWindow::ChessWindow(QWidget* parent)
    : QWidget(parent),
      labels{ boardSize, std::vector<std::shared_ptr<QLabel>>() },
      backgroudLabel{ this }
{
    setFixedSize(800, 800);

    initializeLabels();
    initializeBackground();
}

void ChessWindow::initializeLabels()
{
    for (size_t i = 0; i < boardSize; ++i)
    {
        for (size_t j = 0; j < boardSize; ++j)
        {
            auto newLabel = std::make_shared<QLabel>(this);

            auto xLocation = static_cast<int>(104 + (j * 76));
            auto yLocation = static_cast<int>(62 + (i * 76));

            newLabel->setGeometry(xLocation, yLocation, 59, 51);

            labels[i].push_back(newLabel);
        }
    }
}

void ChessWindow::initializeBackground()
{
    backgroudLabel.setGeometry(79, 30, 642, 644);
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
