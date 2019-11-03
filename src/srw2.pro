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
    Battle/battle.cpp \
    Battle/battleground.cpp \
    DataReader/imageresourcemanager.cpp \
    Music/music.cpp \
    Music/musicmanager.cpp \
    Scene/scene.cpp \
    Scene/scenebeforemain.cpp \
    Scene/scenestart.cpp \
    Scene/scenetitle.cpp \
    UI/button.cpp \
    UI/menu.cpp \
    UI/robotstatusview.cpp \
    main.cpp \
    game.cpp \
    scene_main.cpp \
    rect.cpp \
    maprect.cpp \
    map.cpp \
    robot.cpp \
    people.cpp \
    ai.cpp \
    story.cpp \
    storydiag.cpp \

HEADERS += \
    Battle/battle.h \
    Battle/battleground.h \
    DataReader/config.h \
    DataReader/datahelper.h \
    DataReader/imageresourcemanager.h \
    Music/music.h \
    Music/musicmanager.h \
    Scene/scene.h \
    Scene/scenebeforemain.h \
    Scene/scenestart.h \
    Scene/scenetitle.h \
    UI/button.h \
    UI/menu.h \
    UI/robotstatusview.h \
    common.h \
    game.h \
    scene_main.h \
    rect.h \
    maprect.h \
    map.h \
    robot.h \
    robotproperty.h \
    people.h \
    peopleproperty.h \
    weapon.h \
    ai.h \
    story.h \
    storydiag.h \
    conversation.h \

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
