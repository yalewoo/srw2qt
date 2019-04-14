#-------------------------------------------------
#
# Project created by QtCreator 2018-03-20T19:01:21
#
#-------------------------------------------------

QT       += core gui
QT       += multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = srw2_cpp
TEMPLATE = app

PRECOMPILED_HEADER = stable.h


# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp \
    game.cpp \
    maprect.cpp \
    map.cpp \
    robot.cpp \
    point.cpp \
    button.cpp \
    menu.cpp \
    weapon.cpp \
    battleground.cpp \
    people.cpp \
    board.cpp \
    battle.cpp \
    music.cpp

HEADERS  += \
    game.h \
    maprect.h \
    map.h \
    robot.h \
    point.h \
    button.h \
    menu.h \
    weapon.h \
    battleground.h \
    people.h \
    board.h \
    stable.h \
    battle.h \
    music.h
