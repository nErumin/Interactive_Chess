TEMPLATE = app
TARGET = ChessVisualizer

QT = core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

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
    Pawn.cpp

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
    Pawn.h
