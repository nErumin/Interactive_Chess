TEMPLATE = app
TARGET = ChessVisualizer

QT = core gui
CONFIG += console

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

unix {
    QMAKE_CXXFLAGS += -std=c++17
}

win32 | win64 {
    QMAKE_CXXFLAGS += /std:c++17
}

SOURCES += \
    main.cpp \
    Vector2.cpp \
    Piece.cpp \
    PolarCoordinate.cpp \
    LineMovementStrategy.cpp \
    LShapedMovementStrategy.cpp \
    King.cpp \
    Queen.cpp \
    Bishop.cpp \
    Rook.cpp \
    Knight.cpp \
    Pawn.cpp \
    Cell.cpp \
    NullPiece.cpp \
    Board.cpp \
    Player.cpp \
    ChessGame.cpp \
    ChessController.cpp \
    ChessWindow.cpp \
    Timer.cpp \
    InformationModal.cpp

HEADERS += \
    Vector2.h \
    Piece.h \
    PieceColor.h \
    MovementStrategy.h \
    LineMovementStrategy.h \
    LShapedMovementStrategy.h \
    PolarCoordinate.h \
    MathUtils.h \
    King.h \
    Queen.h \
    Bishop.h \
    Rook.h \
    Knight.h \
    Pawn.h \
    Cell.h \
    CellColor.h \
    NullPiece.h \
    Board.h \
    ChessGame.h \
    PlayerType.h \
    Player.h \
    Observable.h \
    Observer.h \
    ChessController.h \
    ChessWindow.h \
    Timer.h \
    InformationModal.h \
    GameResult.h \
    BoardUtility.h \
    StringUtility.h

RESOURCES += \
    resource.qrc

LIBS += -L$$PWD/lib -lNetworkLibrary

INCLUDEPATH += $$PWD/includes
DESTDIR = binaries
