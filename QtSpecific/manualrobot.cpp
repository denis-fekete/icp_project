#include "manualrobot.h"

ManualRobot::ManualRobot(double x, double y, double radius, double rot,
                     double detRadius, QColor color,
                     std::vector<Rectangle*>* colliders, Simulator* simulator) : BaseRobot(x, y, radius, rot, detRadius, color, colliders, simulator),
    speed(0),turnAngle(0)
{
    cmd = Command::STAY;
    initialized = false;
}


void ManualRobot::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    // Body
    painter->setPen(pen);
    painter->setBrush(color);
    painter->drawEllipse(-sim.getRadius(), -sim.getRadius(), 2 * sim.getRadius(), 2 * sim.getRadius());

    // Center sign
    painter->setPen(QPen(Qt::white, HIGHLIGHTED_PEN_WIDTH));
    painter->drawLine(QLine(-3, -3, 3, 0));
    painter->drawLine(QLine(3, 0, -3, 3));
    painter->drawLine(QLine(-3, 3, -3, -3));

    // Collider
    painter->setPen(QPen(highligtedColor, DEFAULT_PEN_WIDTH));
    painter->setBrush(QBrush(this->color, Qt::BrushStyle::NoBrush));
    painter->drawRect(-sim.getRadius(), -sim.getRadius(),  sim.getRadius() + sim.getDetRadius(), 2 * sim.getRadius());
}

void ManualRobot::simulate()
{
    switch(cmd)
    {
    case Command::STAY:
        break;
    case Command::FORWARD:
        if(!this->sim.obstacleDetection(colliders))
        {
            this->moveRobot(speed);
        }
        else
        {
            qDebug("collision, cannot move");
        }
        break;
    case Command::ROTATE_CLOCK:
        this->rotateRobot(-turnAngle);
        break;
    case Command::ROTATE_ANTICLOCK:
        this->rotateRobot(turnAngle);
        break;
    }
}
