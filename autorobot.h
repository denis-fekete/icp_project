#ifndef AUTOROBOT_H
#define AUTOROBOT_H

#include <robot.h>
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QGraphicsScene>

class AutoRobot : public Robot
{
public:
    AutoRobot(double x, double y, double radius, double rot, double detRadius, QColor color);

    QGraphicsEllipseItem* graphics;
    QColor color;


    void Initialize(QGraphicsScene* scene);
    void Move(double distance);
    void Rotate(double angle);
};

#endif // AUTOROBOT_H
