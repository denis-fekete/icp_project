#include "autorobot.h"

AutoRobot::AutoRobot(double x, double y, double radius, double rot, double detRadius, QColor color) : Robot(x, y, radius, rot, detRadius)
{
    this->color = color;
}


void AutoRobot::Initialize(QGraphicsScene* scene)
{
    QPen robotPen(color);
    robotPen.setWidth(1);

    this->graphics = scene->addEllipse(x - radius / 2, y - radius / 2, radius, radius, robotPen, QBrush(color));
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

    const auto rad = this->radius / 2;
    this->graphics->moveBy(rad, rad);
    this->graphics->setRotation(this->rot);
    this->graphics->moveBy(-rad, -rad);
}
