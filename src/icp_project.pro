QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17 QMAKE_CFLAGS_RELEASE release

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    2DSimulationLib/circle.cpp \
    2DSimulationLib/rectangle.cpp \
    2DSimulationLib/robot.cpp \
    QtSpecific/autorobot.cpp \
    QtSpecific/baserobot.cpp \
    QtSpecific/manualrobot.cpp \
    QtSpecific/obstacle.cpp \
    QtSpecific/savemanager.cpp \
    QtSpecific/simulationcore.cpp \
    QtSpecific/simulator.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    2DSimulationLib/circle.h \
    2DSimulationLib/point.h \
    2DSimulationLib/randomgenerator.h \
    2DSimulationLib/rectangle.h \
    2DSimulationLib/robot.h \
    QtSpecific/autorobot.h \
    QtSpecific/baserobot.h \
    QtSpecific/customscene.h \
    QtSpecific/manualrobot.h \
    QtSpecific/obstacle.h \
    QtSpecific/savemanager.h \
    QtSpecific/simulationcore.h \
    QtSpecific/simulator.h \
    mainwindow.h


FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
