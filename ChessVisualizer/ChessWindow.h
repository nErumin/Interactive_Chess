#ifndef CHESSWINDOW_H
#define CHESSWINDOW_H

#include <QWidget>
#include <QLabel>
#include <memory>

class ChessWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ChessWindow(QWidget* parent = nullptr);
    QLabel& getLabel(size_t row, size_t column) noexcept;
    const QLabel& getLabel(size_t row, size_t column) const noexcept;
private:
    void initializeLabels();
    void initializeBackground();

private:
    std::vector<std::vector<std::shared_ptr<QLabel>>> labels;
    QLabel backgroudLabel;
};

#endif // CHESSWINDOW_H
