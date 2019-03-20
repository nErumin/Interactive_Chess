TEMPLATE = app
TARGET = ChessVisualizer

QT = core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += \
    main.cpp \
    piece.cpp \
    vector2.cpp

HEADERS += \
    piececolor.h \
    piece.h \
    vector2.h
