#include "ChessWindow.h"
#include "ChessController.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ChessController e;

    e.startChess();
    return a.exec();
}
