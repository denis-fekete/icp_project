#include "mainwindow.h"
#include "Tests/testsRunner.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    TestRunner::testRunner();

    QApplication a(argc, argv);
    MainWindow w;

    w.show();
    return a.exec();
}
