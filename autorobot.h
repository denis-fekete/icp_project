#ifndef AUTOROBOT_H
#define AUTOROBOT_H

#include "robot.h"
#include "obstacle.h"
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QTimer>
#include <QTime>
#include <QDebug>

#include <vector>

class AutoRobot : public QObject
{
    Q_OBJECT
protected:
    Robot* sim; // Simulation of physics for robot
    QGraphicsEllipseItem* graphics;
    QGraphicsRectItem* collider;
    std::vector<Obstacle*>* obstacles;

    QColor color;
    double speed;
    QTimer* timer;
    QTime* clock;
public:
    AutoRobot(double x, double y, double radius, double rot, double detRadius, QColor color, double speed, std::vector<Obstacle*>* obstaclesPointer, QTime* clock);
    ~AutoRobot();
    void Initialize(QGraphicsScene* scene);
    void Move(double distance);
    void RotateAroundSelf(double angle);

    Robot* GetSimatationInfo();
    QGraphicsEllipseItem* GetGraphics();
    QGraphicsRectItem* GetCollider();

    void SetUnselected();
    void SetSelected();
    int timeOfSimulationCicle;

public slots:
    void Simulate();
};

#endif // AUTOROBOT_H
