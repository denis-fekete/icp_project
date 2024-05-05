/**
 * @file main.cpp
 * @brief Main file containing main function that will be called by Makefile
 *
 * @author Denis Fekete (xfeket01@fit.vutbr.cz)
 */

#include "mainwindow.h"

#include <QDebug>
#include <QApplication>

int main(int argc, char *argv[])
{
#ifdef QT_DEBUG
    qDebug("Application in DEBUG mode");
#else
    qDebug("Application in RELEASE mode");
#endif

    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("ICP Project - 2D Collision Simulator");

    w.show();
    return a.exec();
}
