#include "autorobot.h"
#include "QPen"
#include "QObject"
#include "QTimer"

AutoRobot::AutoRobot(double x, double y, double radius, double rot,
                     double detRadius, QColor color, double speed,
                     double turnAngle, bool turnRight,
                     std::vector<Rectangle*>* colliders, Simulator* simulator) : BaseRobot(x, y, radius, rot, detRadius, color, colliders, simulator),
    speed(speed),turnAngle(turnAngle), turnDirection((turnRight)? 1 : -1)
{
    initialized = false;
}


void AutoRobot::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    // Body
    painter->setPen(pen);
    painter->setBrush(color);
    painter->drawEllipse(-sim.getRadius(), -sim.getRadius(), 2 * sim.getRadius(), 2 * sim.getRadius());

    // Center sign
    painter->setPen(QPen(Qt::white, HIGHLIGHTED_PEN_WIDTH));
    painter->drawLine(QLine(-3, -3, 3, 0));
    painter->drawLine(QLine(3, 0, -3, 3));

    // Collider
    painter->setPen(QPen(highligtedColor, DEFAULT_PEN_WIDTH));
    painter->setBrush(QBrush(this->color, Qt::BrushStyle::NoBrush));
    painter->drawRect(-sim.getRadius(), -sim.getRadius(),  sim.getRadius() + sim.getDetRadius(), 2 * sim.getRadius());
}

void AutoRobot::simulate()
{
    bool collision = this->sim.obstacleDetection(colliders);
    if(collision)
    {
        this->rotateRobot(turnAngle * turnDirection);
    }
    else
    {
        this->moveRobot(speed);
    }
}
