#include "autorobot.h"


AutoRobot::AutoRobot(double x, double y, double radius, double rot,
                      double detRadius, QColor color, double speed,
                      double turnAngle, bool turnRight,
                      std::vector<Obstacle*>* obstaclesPointer, QTime* clock)
{
    this->sim = new Robot(x, y, radius, rot, detRadius);
    this->speed = speed;
    this->color = color;
    this->turnAngle = turnAngle;
    this->turnDirection = (turnRight) ? 1 : -1;

    this->obstacles = obstaclesPointer;
    this->clock = clock;

    // timer = new QTimer();
    // connect(timer, SIGNAL(timeout()), this, SLOT(Simulate()));
    // timer->start(1);

    connect(this, SIGNAL(qtDummyMove()), this, SLOT(MoveUpdateGraphics()), Qt::QueuedConnection);
    connect(this, SIGNAL(qtDummyRotate()), this, SLOT(RotateUpdateGraphics()), Qt::QueuedConnection);
}

AutoRobot::~AutoRobot()
{
    delete this->sim;
    delete this->timer;
}

void AutoRobot::Initialize(QGraphicsScene* scene)
{
    this->graphics = scene->addEllipse(sim->x - sim->radius / 2, sim->y - sim->radius / 2, sim->radius, sim->radius, QPen(color), QBrush(color));
    this->collider = scene->addRect(sim->colliderFwd.x - sim->colliderFwd.w/2,
                                    sim->colliderFwd.y - sim->colliderFwd.h/2,
                                    sim->colliderFwd.w,
                                    sim->colliderFwd.h,
                                    QPen(QColor(60, 60, 60), 1, Qt::DotLine));

    this->graphics->setTransformOriginPoint(sim->x, sim->y);
    this->collider->setTransformOriginPoint(sim->x, sim->y);
}

void AutoRobot::MoveRobot(double distance)
{
    lastMoveDelta = this->sim->MoveForward(distance);
    emit qtDummyMove();
}

void AutoRobot::MoveUpdateGraphics()
{
    this->graphics->moveBy(lastMoveDelta.x, lastMoveDelta.y);
    this->collider->moveBy(lastMoveDelta.x, lastMoveDelta.y);
}

void AutoRobot::RotateRobot(double angle)
{
    sim->Rotate(angle);
    emit qtDummyRotate();
}

void AutoRobot::RotateUpdateGraphics()
{
    const auto rad = sim->radius / 2;
    this->graphics->moveBy(rad, rad);
    this->graphics->setRotation(sim->rot);
    this->graphics->moveBy(-rad, -rad);

    const auto w2 = sim->colliderFwd.w/2 + sim->radius;
    const auto h2 = sim->colliderFwd.h/2 + sim->radius;
    this->collider->moveBy(w2, h2);
    this->collider->setRotation(this->sim->colliderFwd.rot);
    this->collider->moveBy(-w2, -h2);
}

void AutoRobot::Simulate()
{
    this->MoveRobot(this->speed);

    bool collision = this->sim->ObstacleDetection(obstacles);
    if(collision)
    {
        this->RotateRobot(this->turnAngle * turnDirection);
    }
}

Robot* AutoRobot::GetSimatationInfo()
{
    return this->sim;
}

QGraphicsEllipseItem* AutoRobot::GetGraphics()
{
    return this->graphics;
}

QGraphicsRectItem* AutoRobot::GetCollider()
{
    return this->collider;
}


void AutoRobot::SetSelected()
{
    QColor highlightedColor = color.lighter(30);
    this->graphics->setPen(QPen(highlightedColor, 5));
}

void AutoRobot::SetUnselected()
{
    this->graphics->setPen(QPen(color));
}
