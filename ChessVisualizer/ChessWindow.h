#ifndef CHESSWINDOW_H
#define CHESSWINDOW_H

#include <QWidget>
#include <QLabel>
#include <memory>
#include <mutex>

class ChessWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ChessWindow(QWidget* parent = nullptr);
    QLabel& getLabel(size_t row, size_t column) noexcept;
    const QLabel& getLabel(size_t row, size_t column) const noexcept;

    QLabel& getUpperLabel() noexcept;
    const QLabel& getUpperLabel() const noexcept;
    QLabel& getLowerLabel() noexcept;
    const QLabel& getLowerLabel() const noexcept;

    void setWindowColor(std::tuple<int, int, int, int> colorVector);
private:
    void initializeLabels();
    void initializeBackground();
private:
    mutable std::mutex resMutex;

    std::vector<std::vector<std::shared_ptr<QLabel>>> labels;
    QLabel windowLabel;
    QLabel chessBoardLabel;
    QLabel upperLabel;
    QLabel lowerLabel;
};

#endif // CHESSWINDOW_H
