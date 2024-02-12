#include "autorobot.h"

AutoRobot::AutoRobot(double x, double y, double radius, double rot, double detRadius, QColor color) : Robot(x, y, radius, rot, detRadius)
{
    this->color = color;
}


void AutoRobot::Initialize(QGraphicsScene* scene)
{
    QPen robotPen(color);
    robotPen.setWidth(1);

    scene->addEllipse(x - radius, y - radius, radius, radius, robotPen, QBrush(color));
}

void AutoRobot::Move(double distance)
{
    this->MoveForward(distance);
    this->graphics->setPos(this->x, this->y);
}

void AutoRobot::Rotate(double angle)
{
    this->Rotate(angle);
    this->graphics->setRotation(this->rot);
}
