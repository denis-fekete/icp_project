#include "manualrobot.h"

ManualRobot::ManualRobot(double x, double y, double radius, double rot,
                     double detRadius, QColor color,
                     std::vector<Rectangle*>* colliders,
                     std::vector<Robot*>* robotColliders,
                     Simulator* simulator) :
    BaseRobot(x, y, radius, rot, detRadius, color, 0, 0, 1, colliders, robotColliders, simulator)
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
    painter->setPen(QPen(highlightedColor, DEFAULT_PEN_WIDTH, Qt::DashLine));
    painter->setBrush(QBrush(this->color, Qt::BrushStyle::NoBrush));

    painter->drawLine(0, sim.getRadius(), sim.getDetRadius(), sim.getRadius());
    painter->drawLine(sim.getDetRadius(), sim.getRadius(), sim.getDetRadius(), -sim.getRadius());
    painter->drawLine(sim.getDetRadius(), -sim.getRadius(), 0, -sim.getRadius());

}

void ManualRobot::simulate()
{
    switch(cmd)
    {
    case Command::STAY:
        break;
    case Command::FORWARD:
        if(!this->sim.obstacleDetection(colliders) && !this->sim.robotDetection(robotColliders))
        {
            this->moveRobot(speed);
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
