#include "autorobot.h"
#include "QPen"

AutoRobot::AutoRobot(double x, double y, double radius, double rot,
                     double detRadius, QColor color, double speed,
                     double turnAngle, bool turnRight,
                     std::vector<std::unique_ptr<Obstacle>>& obstacles) : sim(x, y, radius, rot, detRadius), obstacles(obstacles)
{
    this->speed = speed;
    this->color = color;
    this->turnAngle = turnAngle;
    this->turnDirection = (turnRight) ? 1 : -1;

    initialized = false;
}

void AutoRobot::initialize(QGraphicsScene& scene)
{
    this->setTransformOriginPoint(sim.x, sim.y);
    scene.addItem(this);
    initialized = true;
}

QRectF AutoRobot::boundingRect() const
{
    const double adjust = 1;
    return QRectF( -sim.getRadius() - adjust,
                   -sim.getRadius() - adjust,
                    sim.getRadius() + adjust,
                    sim.getRadius() + adjust);
}

QPainterPath AutoRobot::shape() const
{
    QPainterPath path;
    path.addRect(-sim.getRadius(),
                 -sim.getRadius(),
                 sim.getRadius(),
                 sim.getRadius());
    return path;
}

void AutoRobot::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    // Body
    const auto halfRadius = sim.getRadius() / 2;
    painter->setBrush(color);
    painter->drawEllipse(sim.getX() - halfRadius, sim.getY() - halfRadius, sim.getRadius(), sim.getRadius());

    // Collider
    QPainterPath path(QPoint(sim.colliderFwd.LB.x, sim.colliderFwd.LB.y));
    path.lineTo(QPoint(sim.colliderFwd.RB.x, sim.colliderFwd.RB.y));
    path.lineTo(QPoint(sim.colliderFwd.RT.x, sim.colliderFwd.RT.y));
    path.lineTo(QPoint(sim.colliderFwd.LT.x, sim.colliderFwd.LT.y));
    path.lineTo(QPoint(sim.colliderFwd.LB.x, sim.colliderFwd.LB.y));
    painter->setBrush(QBrush(this->color, Qt::BrushStyle::NoBrush));
    painter->drawPath(path);
}

void AutoRobot::simulate()
{
    if(!initialized) return;

    bool collision = this->sim.obstacleDetection(obstacles);
    if(collision)
    {
        this->rotateRobot(turnAngle * turnDirection);
    }
    else
    {
        this->moveRobot(speed);
    }
}

Robot& AutoRobot::getSimulationInfo()
{
    return this->sim;
}


void AutoRobot::setSelected()
{
    // QColor highlightedColor = color.lighter(30);
    // this->setPen(QPen(highlightedColor, 5));
}

void AutoRobot::setUnselected()
{
    // this->setPen(QPen(color));
}

void AutoRobot::addRobotToWorld( double x, double y, double radius, double rot,
                                double detRadius, QColor color, double speed,
                                double turnAngle, bool turnRight,
                                std::vector<std::unique_ptr<Obstacle>>& obstaclesPointer,
                                std::vector<std::unique_ptr<AutoRobot>>& robots, QGraphicsScene& scene)
{
    robots.push_back(std::make_unique<AutoRobot> ( x, y, radius, rot, detRadius, color, speed, turnAngle, turnRight, obstaclesPointer));

    robots.back()->initialize(scene);
}
