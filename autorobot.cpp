#include "autorobot.h"

AutoRobot::AutoRobot(double x, double y, double radius, double rot, double detRadius, QColor color) : Robot(x, y, radius, rot, detRadius)
{
    QPen robotPen(color);
    robotPen.setWidth(1);
    this->graphics = new QGraphicsEllipseItem(x, y, radius, radius, robotPen, QBrush(color));
}

