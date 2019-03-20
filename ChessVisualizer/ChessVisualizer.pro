TEMPLATE = app
TARGET = ChessVisualizer

QT = core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += \
    main.cpp \
    Vector2.cpp \
    Piece.cpp \
    MovementStrategy.cpp

HEADERS += \
    Vector2.h \
    Piece.h \
    PieceColor.h \
    MovementStrategy.h \
