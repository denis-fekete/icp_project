#include "manualrobot.h"

ManualRobot::ManualRobot(double x, double y, double radius, double rot,
                     double detRadius, QColor color,
                     std::vector<Rectangle*>* colliders,
                     std::vector<Robot*>* robotColliders,
                     Simulator* simulator, double* spaceWidth, double* spaceHeight) :
    BaseRobot(x, y, radius, rot, detRadius, color, 0, 0, 1, colliders, robotColliders, simulator, spaceWidth, spaceHeight)
{
    cmd = Command::STAY;
    initialized = false;
}

void ManualRobot::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{

    // Body
    painter->setPen(pen);
    painter->setBrush(color);
    const int flooredRadius = std::floor(sim.getRadius());
    const int flooredDetRadius = std::floor(sim.getDetRadius());

    painter->drawEllipse(-sim.getRadius(), -sim.getRadius(), 2 * sim.getRadius(), 2 * sim.getRadius());

    // Center sign
    painter->setPen(QPen(Qt::white, HIGHLIGHTED_PEN_WIDTH));
    painter->drawLine(QLine(-3, -3, 3, 0));
    painter->drawLine(QLine(3, 0, -3, 3));
    painter->drawLine(QLine(-3, 3, -3, -3));

    // Collider
    painter->setPen(QPen(highlightedColor, 2, Qt::SolidLine));
    painter->setBrush(QBrush(this->color, Qt::BrushStyle::NoBrush));


    painter->drawLine(0, flooredRadius, flooredDetRadius, flooredRadius);
    painter->drawLine(flooredDetRadius,flooredRadius, flooredDetRadius, -flooredRadius);
    painter->drawLine(flooredDetRadius, -flooredRadius, 0, -flooredRadius);

}

void ManualRobot::simulate()
{
    switch(cmd)
    {
    case Command::STAY:
        break;
    case Command::FORWARD:
        if( !this->sim.obstacleDetection(colliders) &&
            !this->sim.robotDetection(robotColliders) &&
            !isOutsideSimulation())
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
