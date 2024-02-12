#include "autorobot.h"

AutoRobot::AutoRobot(double x, double y, double radius, double rot, double detRadius, QColor color) : Robot(x, y, radius, rot, detRadius)
{
    this->color = color;
}


void AutoRobot::Initialize(QGraphicsScene* scene)
{
    QPen robotPen(color);
    robotPen.setWidth(1);

    this->graphics = scene->addEllipse(x - radius, y - radius, radius, radius, robotPen, QBrush(color));
    this->graphics->setTransformOriginPoint(this->x, this->y);
}

void AutoRobot::Move(double distance)
{
    this->MoveForward(distance);
    this->graphics->setPos(this->x, this->y);
}

void AutoRobot::RotateAroundSelf(double angle)
{
    this->Rotate(angle);
    this->graphics->setRotation(this->rot);
}
