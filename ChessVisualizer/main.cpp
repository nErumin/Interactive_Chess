#include "ChessWindow.h"
#include "ChessController.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);
    ChessController controller;

    controller.startChess();
    application.exec();

    controller.finalize();
    return 0;
}
