#ifndef SAVEMANAGER_H
#define SAVEMANAGER_H

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
    SaveManager(std::vector<std::unique_ptr<AutoRobot>>& robots,
                std::vector<std::unique_ptr<Obstacle>>& obstacles, QWidget* widget,
                QGraphicsScene& scene, AutoRobot** activeRobot,
                Obstacle** activeObstacle);

    void loadFromFile();
    void saveToFile();
private:
    std::vector<std::unique_ptr<AutoRobot>>& robots;
    std::vector<std::unique_ptr<Obstacle>>& obstacles;
    AutoRobot** activeRobot;
    Obstacle** activeObstacle;

    QWidget* widget;
    QGraphicsScene& scene;


    returnType readRobots();
    returnType readRobot();

    returnType readObstacles();
    returnType readObstacle();

    QXmlStreamReader xmlReader;
    QXmlStreamWriter xmlWriter;

};

#endif // SAVEMANAGER_H
