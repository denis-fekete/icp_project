/**
 * @file baserobot.cpp
 * @brief Implementation of BaseRobot class methods
 *
 * @author Denis Fekete (xfeket01@fit.vutbr.cz)
 */

#include "baserobot.h"
#include "simulator.h"

BaseRobot::BaseRobot(double x, double y, double radius, double rot,
          double detRadius, QColor color, double speed,
          double turnSpeed, short turnDirection,
          std::vector<Rectangle*>* colliders,
          std::vector<Robot*>* robotColliders,
          Simulator* simulator, double* spaceWidth, double* spaceHeight) :
    sim(x, y, radius, rot, detRadius), colliders(colliders),
    robotColliders(robotColliders), color(color), simulator(simulator),
    speed(speed), turnSpeed(turnSpeed), turnDirection(turnDirection),
    spaceWidth(spaceWidth), spaceHeight(spaceHeight)
{
    initialized = false;
}

void BaseRobot::initialize()
{
    rotateRobot(0);
    sim.getColliderFwd()->rotate(0);

    // item can be moved by mouse
    setFlag(QGraphicsItem::ItemIsMovable);
    // can be selected, for moving multiple objects at one
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);
    // // anti-aliasing
    setTransformationMode(Qt::SmoothTransformation);
    // set origin point to 0,0
    setTransformOriginPoint(0, 0);
    // make this appear on top of all objects, obstacles have 1
    setZValue(2);
    initialized = true;

    highlightedColor.setRed(std::min(color.red() + 30, 240));
    highlightedColor.setGreen(std::min(color.green() + 30, 240));
    highlightedColor.setBlue(std::min(color.blue() + 30, 240));
}

QRectF BaseRobot::boundingRect() const
{
    const double adj = 1;
    return QRectF(-sim.getRadius() - adj, -sim.getRadius() - adj,
                  2* sim.getRadius() +  sim.getDetRadius() + adj,
                  2*sim.getRadius() + adj);
}

QPainterPath BaseRobot::shape() const
{
    QPainterPath path;
    path.addRect(-sim.getRadius(), -sim.getRadius(),
                 2 * sim.getRadius() + sim.getDetRadius(),
                 2 * sim.getRadius());
    return path;
}

QVariant BaseRobot::itemChange(GraphicsItemChange change, const QVariant &value)
{
    auto flags = this->flags();
    if (change == GraphicsItemChange::ItemPositionHasChanged)
    {
        QPointF newPosition = value.toPointF();
        if (flags.testFlag(QGraphicsItem::ItemIsMovable))
        {
            Point p(newPosition.x(), newPosition.y());
            this->sim.moveTo(p);
            simulator->setActiveRobot(this);
        }

        return newPosition;
    }
    else if (change == GraphicsItemChange::ItemSelectedChange)
    {
        simulator->setActiveRobot(this);
    }

    return QGraphicsItem::itemChange(change, value);
}

void BaseRobot::positionUpdate()
{
    // Correctly rotate
    this->setPos(0, 0);
    this->setRotation(sim.getRotation());
    this->setPos(sim.getX(), sim.getY());
}


void BaseRobot::setSelected()
{
    pen.setColor(highlightedColor);
    pen.setWidth(HIGHLIGHTED_PEN_WIDTH);
}

void BaseRobot::setUnselected()
{
    pen.setColor(color);
    pen.setWidth(DEFAULT_PEN_WIDTH);
}

void BaseRobot::advance(int step)
{
    if(!step)
        return;

    const double x = sim.getX();
    const double y = sim.getY();
    this->setPos(0, 0);
    this->setRotation(sim.getRotation());
    this->setPos(x, y);
}

void BaseRobot::updateValues(double x, double y, double radius, double rot,
                             double detRadius, QColor color, double speed,
                             double turnSpeed, short turnDirection)
{
    this->turnDirection = turnDirection;
    this->turnSpeed = turnSpeed;
    this->color = color;
    this->speed = speed;

    sim.setRadius(radius);
    sim.setDetRadius(detRadius);

    sim.getColliderFwd()->setPos(sim.getX() + sim.getRadius(), sim.getY());
    sim.getColliderFwd()->setW(sim.getDetRadius());
    sim.getColliderFwd()->setH(2 * sim.getRadius());

    sim.moveTo(Point(x, y));
    sim.setRotation(rot);

    // update rotation of collider by rotating by 0 angle
    sim.rotate(0);

    advance(1);
}
