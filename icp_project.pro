QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Simulation/circle.cpp \
    Simulation/line.cpp \
    Simulation/point.cpp \
    Simulation/rectangle.cpp \
    Simulation/robot.cpp \
    Simulation/simulationcore.cpp \
    Simulation/simulator.cpp \
    Tests/pointtest.cpp \
    Tests/robottest.cpp \
    Tests/test.cpp \
    Visualization/autorobot.cpp \
    Visualization/obstacle.cpp \
    Visualization/obstacle_conf_menu.cpp \
    Visualization/robot_conf_menu.cpp \
    Visualization/world_conf_menu.cpp \
    main.cpp \
    mainwindow.cpp


HEADERS += \
    Simulation/circle.h \
    Simulation/line.h \
    Simulation/point.h \
    Simulation/randomgenerator.h \
    Simulation/rectangle.h \
    Simulation/robot.h \
    Simulation/simulationcore.h \
    Simulation/simulator.h \
    Tests/pointtest.h \
    Tests/robottest.h \
    Tests/test.h \
    Tests/testsRunner.h \
    Visualization/autorobot.h \
    Visualization/obstacle.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
