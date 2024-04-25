#include "autorobot.h"
#include "QPen"
#include "QObject"
#include "QTimer"

AutoRobot::AutoRobot(double x, double y, double radius, double rot,
                     double detRadius, QColor color, double speed,
                     double turnAngle, bool turnRight,
                     std::vector<Rectangle*>* colliders, AutoRobot **activeRobot) :
    sim(x, y, radius, rot, detRadius), colliders(colliders), color(color), speed(speed),
    turnAngle(turnAngle), turnDirection((turnRight)? 1 : -1), activeRobot(activeRobot)
{
    initialized = false;
}


void AutoRobot::initialize()
{
    this->setFlag(QGraphicsItem::ItemIsMovable);
    this->setFlag(QGraphicsItem::ItemSendsGeometryChanges);
    this->setTransformOriginPoint(0, 0);
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

void AutoRobot::positionUpdate()
{
    // Correctly rotate
    this->setPos(0, 0);
    this->setRotation(sim.getRotation());
    this->setPos(sim.getX(), sim.getY());
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
    bool collision = this->sim.obstacleDetection(colliders);
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
