#ifndef AUTOROBOT_H
#define AUTOROBOT_H

#include <robot.h>
#include <QGraphicsItem>
#include <QGraphicsView>

class AutoRobot : public Robot
{
public:
    AutoRobot(double x, double y, double radius, double rot, double detRadius, QColor color);
    //Robot(double x, double y, double radius, double rot, double detRadius);
private:
    QGraphicsEllipseItem* graphics;
};

#endif // AUTOROBOT_H
