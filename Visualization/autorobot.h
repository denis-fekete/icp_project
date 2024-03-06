#ifndef AUTOROBOT_H
#define AUTOROBOT_H

#include <QObject>
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QTimer>
#include <QTime>
#include <QDebug>

#include <vector>

#include "obstacle.h"
#include "../Simulation/robot.h"

class AutoRobot : public QObject
{
    Q_OBJECT
protected:
    Robot* sim; // Simulation of physics for robot
    QGraphicsEllipseItem* graphics;
    QGraphicsRectItem* collider;
    std::vector<Obstacle*>* obstacles;

    // Robot parameters
    QColor color;
    double speed;
    double turnAngle;
    short signed int turnDirection;

    // Internal
    QTimer* timer;

public:
    AutoRobot(double x, double y, double radius, double rot,
              double detRadius, QColor color, double speed,
              double turnAngle, bool turnRight,
              std::vector<Obstacle*>* obstaclesPointer);

    ~AutoRobot();
    void Initialize(QGraphicsScene* scene);

    Robot* GetSimatationInfo();
    QGraphicsEllipseItem* GetGraphics();
    QGraphicsRectItem* GetCollider();

    void SetUnselected();
    void SetSelected();

    void Simulate();

    void MoveRobot(double distance);
    void RotateRobot(double angle);

    Point lastMoveDelta;

signals:
    void qtDummyMove();
    void qtDummyRotate();


public slots:
    void MoveUpdateGraphics();
    void RotateUpdateGraphics();
};

#endif // AUTOROBOT_H
