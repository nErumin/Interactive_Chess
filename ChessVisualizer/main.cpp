#include "ChessWindow.h"
#include "ChessController.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    try
    {
        QApplication application(argc, argv);
        ChessController controller;

        controller.startChess();
        application.exec();

        controller.finalize();
    }
    catch (const std::exception& error)
    {
        std::cout << "Warning: " << error.what() << std::endl;
    }

    return 0;
}
