#include "manualrobot.h"

ManualRobot::ManualRobot(double x, double y, double radius, double rot,
                     double detRadius, QColor color,
                     std::vector<Rectangle*>* colliders, BaseRobot **activeRobot) : BaseRobot(x, y, radius, rot, detRadius, color, colliders, activeRobot)
{
    initialized = false;
}


void ManualRobot::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
// #define POINT_REPRE
#ifndef POINT_REPRE
    // Body
    painter->setBrush(brush);
    painter->drawEllipse(-sim.getRadius(), -sim.getRadius(), 2 * sim.getRadius(), 2 * sim.getRadius());
    painter->drawEllipse(sim.getRadius() - 6, -3, 6, 6);
    // Collider
    painter->setBrush(QBrush(this->color, Qt::BrushStyle::NoBrush));

    painter->drawRect(-sim.getRadius(), -sim.getRadius(),  sim.getRadius() + sim.getDetRadius(), 2 * sim.getRadius());

// painter->drawEllipse(-sim.getDetRadius(), -sim.getDetRadius(), 2 * sim.getDetRadius(), 2 * sim.getDetRadius()); //DEBUG:
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

void ManualRobot::simulate()
{

    bool collision = this->sim.obstacleDetection(colliders);
    if(collision)
    {
    }
    else
    {
    }
}
