#ifndef SAVEMANAGER_H
#define SAVEMANAGER_H

#include "2DSimulationLib/simulator.h"
#include "QIODevice"
#include "QFile"
#include "QXmlStreamReader"
#include "QXmlStreamWriter"
#include <qcoreapplication.h>
#include "QDebug"
#include "QFileDialog"

#include "QtSpecific/autorobot.h"
#include "QtSpecific/obstacle.h"

using returnType = enum {err, dontReadNext, readNext};

class SaveManager
{
public:
    SaveManager(Simulator& simulator, QWidget* widget);

    void loadFromFile();
    void saveToFile();
private:
    Simulator& simulator;

    QWidget* widget;


    returnType readRobots();
    returnType readRobot();

    returnType readObstacles();
    returnType readObstacle();

    QXmlStreamReader xmlReader;
    QXmlStreamWriter xmlWriter;

};

#endif // SAVEMANAGER_H
