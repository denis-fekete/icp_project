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

class SaveManager
{
public:
    SaveManager(std::vector<std::unique_ptr<AutoRobot>>& robots,
                std::vector<std::unique_ptr<Obstacle>>& obstacles, QWidget* widget,
                QGraphicsScene& scene);

    void loadFromFile();
    void saveToFile();
private:
    std::vector<std::unique_ptr<AutoRobot>>& robots;
    std::vector<std::unique_ptr<Obstacle>>& obstacles;
    QWidget* widget;
    QGraphicsScene& scene;

    bool readRobots();
    bool readRobot();

    bool readObstacles();
    bool readObstacle();

    QXmlStreamReader xmlReader;
    QXmlStreamWriter xmlWriter;
};

#endif // SAVEMANAGER_H
