QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    2DSimulationLib/circle.cpp \
    2DSimulationLib/point.cpp \
    2DSimulationLib/rectangle.cpp \
    2DSimulationLib/robot.cpp \
    2DSimulationLib/simulationcore.cpp \
    2DSimulationLib/simulator.cpp \
    QtSpecific/autorobot.cpp \
    QtSpecific/baserobot.cpp \
    QtSpecific/manualrobot.cpp \
    QtSpecific/obstacle.cpp \
    QtSpecific/savemanager.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    2DSimulationLib/circle.h \
    2DSimulationLib/point.h \
    2DSimulationLib/randomgenerator.h \
    2DSimulationLib/rectangle.h \
    2DSimulationLib/robot.h \
    2DSimulationLib/simulationcore.h \
    2DSimulationLib/simulator.h \
    QtSpecific/autorobot.h \
    QtSpecific/baserobot.h \
    QtSpecific/customscene.h \
    QtSpecific/manualrobot.h \
    QtSpecific/obstacle.h \
    QtSpecific/savemanager.h \
    mainwindow.h

Release:DESTDIR = build/release
Release:OBJECTS_DIR = build/release/obj
Release:MOC_DIR = build/release/moc
Release:RCC_DIR = build/release/rcc
Release:UI_DIR = build/release/.ui

Debug:DESTDIR = build/debug
Debug:OBJECTS_DIR = build/debug/obj
Debug:MOC_DIR = build/debug/moc
Debug:RCC_DIR = build/debug/rcc
Debug:UI_DIR = build/debug/ui

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
