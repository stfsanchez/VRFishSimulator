#-------------------------------------------------
#
# Project created by QtCreator 2013-04-15T10:17:57
#
#-------------------------------------------------

QT += core network gui widgets

TARGET = VRFish2D
TEMPLATE = app
CONFIG += static
CONFIG += c++11
CONFIG += console

SOURCES += main.cpp\
    Agent/circularcontroller.cpp \
    Agent/controller2d.cpp \
    Agent/controller2dconstants.cpp \
    Agent/controller3d.cpp \
    Agent/controller3dconstants.cpp \
    Agent/fish.cpp \
    Agent/fixedpositioncontroller.cpp \
    Agent/function.cpp \
    Agent/networkcontroller.cpp \
    Agent/replaycontroller.cpp \
    Agent/rosacecontroller.cpp \
    Core/fishtank.cpp \
    Graphics/spritebackground.cpp \
    Graphics/spritecenter.cpp \
    Graphics/spritefish.cpp \
    Graphics/spritegrid.cpp \
    Graphics/spriteplainbackground.cpp \
    Graphics/spritetank.cpp \
    Graphics/viewmodifier.cpp \
    Tools/datamanagerconstants.cpp \
    Tools/datamanager.cpp \
    Tools/rngconstants.cpp \
    Tools/rngsingleton.cpp \
    globalconstants.cpp \
    Core/simulator.cpp \
    Graphics/sprite.cpp \
    mainwindow.cpp \
    Tools/networkmanager.cpp \
    Tools/parser.cpp \
    Agent/controller.cpp \
    Tools/udpreceiver.cpp \
    Tools/udpsender.cpp \
    Tools/utils.cpp

HEADERS  += \
    Agent/circularcontroller.h \
    Agent/controller2d.h \
    Agent/controller2dconstants.h \
    Agent/controller3d.h \
    Agent/controller3dconstants.h \
    Agent/fish.h \
    Agent/fixedpositioncontroller.h \
    Agent/function.h \
    Agent/networkcontroller.h \
    Agent/replaycontroller.h \
    Agent/rosacecontroller.h \
    Core/fishtank.h \
    Core/simulator.h \
    Graphics/graphicconstants.h \
    Graphics/sprite.h \
    Graphics/spritebackground.h \
    Graphics/spritecenter.h \
    Graphics/spritefish.h \
    Graphics/spritegrid.h \
    Graphics/spriteplainbackground.h \
    Graphics/spritetank.h \
    Graphics/viewmodifier.h \
    Tools/datamanagerconstants.h \
    Tools/datamanager.h \
    Tools/rngconstants.h \
    Tools/rngsingleton.h \
    globalconstants.h \
    mainwindow.h \
    Tools/networkmanager.h \
    Tools/parser.h \
    Agent/controller.h \
    Tools/udpreceiver.h \
    Tools/udpsender.h \
    Tools/utils.h

FORMS    += \
    mainwindow.ui



