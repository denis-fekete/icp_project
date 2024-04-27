#include "autorobot.h"
#include "QPen"
#include "QObject"
#include "QTimer"

AutoRobot::AutoRobot(double x, double y, double radius, double rot,
                     double detRadius, QColor color, double speed,
                     double turnAngle, bool turnRight,
                     std::vector<Rectangle*>* colliders,
                     std::vector<Robot*>* robotColliders,
                     Simulator* simulator) : BaseRobot(x, y, radius, rot, detRadius, color, colliders, robotColliders, simulator),
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
    painter->setPen(QPen(highlightedColor, DEFAULT_PEN_WIDTH, Qt::DashLine));
    painter->setBrush(QBrush(this->color, Qt::BrushStyle::NoBrush));

    painter->drawLine(0, sim.getRadius(), sim.getDetRadius(), sim.getRadius());
    painter->drawLine(sim.getDetRadius(), sim.getRadius(), sim.getDetRadius(), -sim.getRadius());
    painter->drawLine(sim.getDetRadius(), -sim.getRadius(), 0, -sim.getRadius());
}

void AutoRobot::simulate()
{
    bool collision = this->sim.obstacleDetection(colliders);
    bool collision2 = this->sim.robotDetection(robotColliders);
    if(collision || collision2)
    {
        this->rotateRobot(turnAngle * turnDirection);
    }
    else
    {
        this->moveRobot(speed);
    }
}
