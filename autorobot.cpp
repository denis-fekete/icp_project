#include "autorobot.h"

AutoRobot::AutoRobot(double x, double y, double radius, double rot, double detRadius, QColor color, double speed, std::vector<Rectangle*>* obstaclesPointer)
{
    this->sim = new Robot(x, y, radius, rot, detRadius);
    this->speed = speed;
    this->color = color;

    this->obstacles = obstaclesPointer;

    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(Simulate()));
    timer->start(20);
}


void AutoRobot::Initialize(QGraphicsScene* scene)
{
    QPen robotPen(color);
    robotPen.setWidth(1);

    this->graphics = scene->addEllipse(sim->x - sim->radius / 2, sim->y - sim->radius / 2, sim->radius, sim->radius, robotPen, QBrush(color));
    this->graphics->setTransformOriginPoint(sim->x, sim->y);
}

void AutoRobot::Move(double distance)
{
    sim->MoveForward(distance);
    this->graphics->setPos(sim->x, sim->y);
}

void AutoRobot::RotateAroundSelf(double angle)
{
    sim->Rotate(angle);

    const auto rad = sim->radius / 2;
    this->graphics->moveBy(rad, rad);
    this->graphics->setRotation(sim->rot);
    this->graphics->moveBy(-rad, -rad);
}

void AutoRobot::Simulate()
{
    this->Move(1);
    bool collision = this->sim->ObstacleDetection(*obstacles);

    if(collision)
    {
        this->RotateAroundSelf(90);
    }
}
