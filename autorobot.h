#ifndef AUTOROBOT_H
#define AUTOROBOT_H

#include "robot.h"
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QTimer>
#include <vector>

class AutoRobot : public QObject
{
    Q_OBJECT
protected:
    Robot* sim; // Simulation of physics for robot
    QGraphicsEllipseItem* graphics;
    std::vector<Rectangle*>* obstacles;

    QColor color;
    double speed;
    QTimer* timer;

public:
    AutoRobot(double x, double y, double radius, double rot, double detRadius, QColor color, double speed, std::vector<Rectangle*>* obstaclesPointer);
    void Initialize(QGraphicsScene* scene);
    void Move(double distance);
    void RotateAroundSelf(double angle);


public slots:
    void Simulate();
};

#endif // AUTOROBOT_H
