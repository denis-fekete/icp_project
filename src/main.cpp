#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("ICP Project - 2D Collision Simulator");

    w.show();
    return a.exec();
}
