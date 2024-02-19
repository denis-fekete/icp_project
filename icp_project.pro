QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    autorobot.cpp \
    main.cpp \
    mainwindow.cpp \
    obstacle.cpp \
    obstacle_conf_menu.cpp \
    robot.cpp \
    circle.cpp \
    rectangle.cpp \
    point2d.cpp \
    robot_conf_menu.cpp \
    simulator.cpp \
    world_conf_menu.cpp


HEADERS += \
    autorobot.h \
    mainwindow.h \
    obstacle.h \
    robot.h \
    circle.h \
    rectangle.h \
    point2d.h \
    simulator.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target