#include "baserobot.h"
#include "simulator.h"

BaseRobot::BaseRobot(double x, double y, double radius, double rot,
          double detRadius, QColor color, double speed,
          double turnAngle, short turnDirection,
          std::vector<Rectangle*>* colliders,
          std::vector<Robot*>* robotColliders,
          Simulator* simulator, double* spaceWidth, double* spaceHeight) :
    sim(x, y, radius, rot, detRadius), colliders(colliders),
    robotColliders(robotColliders), color(color), simulator(simulator),
    speed(speed), turnAngle(turnAngle), turnDirection(turnDirection),
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

    this->setPos(0, 0);
    this->setRotation(sim.getRotation());
    this->setPos(sim.getX(), sim.getY());
}
