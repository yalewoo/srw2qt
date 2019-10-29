#-------------------------------------------------
#
# Project created by QtCreator 2019-05-18T16:15:57
#
#-------------------------------------------------

QT       += core gui
QT       += multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = srw2
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
    game.cpp \
    scene_main.cpp \
    rect.cpp \
    maprect.cpp \
    map.cpp \
    scene.cpp \
    robot.cpp \
    people.cpp \
    robotstatusview.cpp \
    button.cpp \
    menu.cpp \
    music.cpp \
    point.cpp \
    battle.cpp \
    battleground.cpp \
    ai.cpp \
    story.cpp \
    storydiag.cpp \
    imageresourcemanager.cpp

HEADERS += \
    game.h \
    scene_main.h \
    datahelper.h \
    rect.h \
    maprect.h \
    map.h \
    config.h \
    scene.h \
    robot.h \
    robotproperty.h \
    people.h \
    peopleproperty.h \
    robotstatusview.h \
    weapon.h \
    button.h \
    menu.h \
    music.h \
    point.h \
    battle.h \
    battleground.h \
    ai.h \
    story.h \
    storydiag.h \
    conversation.h \
    imageresourcemanager.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
