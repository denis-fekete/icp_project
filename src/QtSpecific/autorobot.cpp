/**
 * @file autorobot.cpp
 * @brief Implementation of AutoRobot class methods
 *
 * @author Denis Fekete (xfeket01@fit.vutbr.cz)
 */

#include "autorobot.h"
#include "QPen"
#include "QObject"
#include "QTimer"

/*
    BaseRobot(double x, double y, double radius, double rot,
              double detRadius, QColor color, double speed,
              double turnSpeed, bool turnRight,
              std::vector<Rectangle*>* colliders,
              std::vector<Robot*>* robotColliders,
              Simulator* simulator);
*/

AutoRobot::AutoRobot(double x, double y, double radius, double rot,
                     double detRadius, QColor color, double speed,
                     double turnSpeed, short turnDirection,
                     std::vector<Rectangle*>* colliders,
                     std::vector<Robot*>* robotColliders,
                     Simulator* simulator, double* spaceWidth, double* spaceHeight) :
    BaseRobot(x, y, radius, rot, detRadius, color, speed, turnSpeed, turnDirection, colliders, robotColliders, simulator, spaceWidth, spaceHeight)
{
    initialized = false;
}

void AutoRobot::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    // Body
    painter->setPen(pen);
    painter->setBrush(color);


    const int flooredRadius = std::floor(sim.getRadius());
    const int flooredDetRadius = std::floor(sim.getDetRadius());

    painter->drawEllipse(-flooredRadius, -flooredRadius, 2 * flooredRadius, 2 * flooredRadius);

    // Center sign
    painter->setPen(QPen(Qt::white, HIGHLIGHTED_PEN_WIDTH));
    painter->drawLine(QLine(-3, -3, 3, 0));
    painter->drawLine(QLine(3, 0, -3, 3));

    // Collider
    // painter->setPen(QPen(highlightedColor, DEFAULT_PEN_WIDTH, Qt::DashLine));
    painter->setPen(QPen(highlightedColor, 2, Qt::SolidLine));
    painter->setBrush(QBrush(this->color, Qt::BrushStyle::NoBrush));

    painter->drawLine(0, flooredRadius, flooredDetRadius, flooredRadius);
    painter->drawLine(flooredDetRadius,flooredRadius, flooredDetRadius, -flooredRadius);
    painter->drawLine(flooredDetRadius, -flooredRadius, 0, -flooredRadius);
}

void AutoRobot::simulate()
{
    if( sim.obstacleDetection(colliders) ||
        sim.robotDetection(robotColliders) ||
        isOutsideSimulation())
    {
        this->rotateRobot(turnSpeed * turnDirection);
    }
    else
    {
        this->moveRobot(speed);
    }
}
