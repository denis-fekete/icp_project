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
    this->setFlag(QGraphicsItem::ItemIsMovable);
    this->setFlag(QGraphicsItem::ItemSendsGeometryChanges);
    this->setTransformOriginPoint(0, 0);
    scene.addItem(this);
    initialized = true;

    this->brush = QBrush(color);
}

QRectF AutoRobot::boundingRect() const
{
    const double adj = 1;
    return QRectF(-sim.getRadius() - adj, -sim.getRadius() - adj, sim.getRadius() + adj, sim.getRadius() + adj);
}

QPainterPath AutoRobot::shape() const
{
    QPainterPath path;
    path.addEllipse(-sim.getRadius(), -sim.getRadius(), sim.getRadius(), sim.getRadius());
    path.addRect(-sim.getDetRadius(), -sim.getDetRadius(), sim.getRadius(), sim.getRadius());
    return path;
}

void AutoRobot::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
// #define POINT_REPRE
#ifndef POINT_REPRE
    // Correctly rotate
    this->setPos(0, 0);
    this->setRotation(sim.getRotation());
    this->setPos(sim.getX(), sim.getY());

    // Body
    painter->setBrush(brush);
    painter->drawEllipse(-sim.getRadius(), -sim.getRadius(), 2 * sim.getRadius(), 2 * sim.getRadius());

    // Collider
    painter->setBrush(QBrush(this->color, Qt::BrushStyle::NoBrush));

    painter->drawRect(-sim.getRadius(), -sim.getRadius(),  sim.getRadius() + sim.getDetRadius(), 2 * sim.getRadius());

    painter->drawEllipse(-sim.getDetRadius(), -sim.getDetRadius(), 2 * sim.getDetRadius(), 2 * sim.getDetRadius()); //DEBUG:
#else
    painter->setBrush(QBrush(this->color, Qt::BrushStyle::NoBrush));

    QPainterPath path;

    path.addEllipse(sim.x - sim.getRadius(), sim.y -sim.getRadius(), 2 * sim.getRadius(), 2 * sim.getRadius());
    path.addEllipse(sim.colliderFwd.x - 2, sim.colliderFwd.y - 2, 4, 4);

    path.lineTo(sim.colliderFwd.LB.x, sim.colliderFwd.LB.y);
    path.lineTo(sim.colliderFwd.RB.x, sim.colliderFwd.RB.y);
    path.lineTo(sim.colliderFwd.RT.x, sim.colliderFwd.RT.y);
    path.lineTo(sim.colliderFwd.LT.x, sim.colliderFwd.LT.y);
    path.lineTo(sim.colliderFwd.LB.x, sim.colliderFwd.LB.y);
    painter->drawPath(path);
#endif
}

void AutoRobot::simulate()
{
#ifdef USE_QT_COLLISION
    if(!initialized) return;

    bool collision = false;
    for(size_t i = 0; i < obstacles.size(); i++)
    {
        Obstacle* obs = obstacles.at(i).get();
        if(this->collidesWithItem(obs))
        {
            collision = true;
            break;
        }
    }

    if(collision)
    {
        this->rotateRobot(turnAngle * turnDirection);
    }
    else
    {
        this->moveRobot(speed);
    }

#else
    bool collision = this->sim.obstacleDetection(obstacles);
    if(collision)
    {
        this->rotateRobot(turnAngle * turnDirection);
    }
    else
    {
        this->moveRobot(speed);
    }
#endif
}


void AutoRobot::setSelected()
{
    brush.setStyle(Qt::Dense3Pattern);
}

void AutoRobot::setUnselected()
{
    brush.setStyle(Qt::SolidPattern);

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
