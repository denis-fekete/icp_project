#include "baserobot.h"
#include "../2DSimulationLib/simulator.h"
BaseRobot::BaseRobot(double x, double y, double radius, double rot,
                     double detRadius, QColor color,
                     std::vector<Rectangle*>* colliders, Simulator* simulator) :
    sim(x, y, radius, rot, detRadius), colliders(colliders), color(color), simulator(simulator)
{
    initialized = false;
}

void BaseRobot::initialize()
{
    this->setFlag(QGraphicsItem::ItemIsMovable);
    this->setFlag(QGraphicsItem::ItemSendsGeometryChanges);
    this->setFlag(QGraphicsItem::ItemIsSelectable);
    this->setTransformationMode(Qt::SmoothTransformation);
    this->setTransformOriginPoint(0, 0);
    initialized = true;


    highligtedColor.setRed(std::min(color.red() + 30, 240));
    highligtedColor.setGreen(std::min(color.green() + 30, 240));
    highligtedColor.setBlue(std::min(color.blue() + 30, 240));
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
    pen.setColor(highligtedColor);
    pen.setWidth(HIGHLIGHTED_PEN_WIDTH);
}

void BaseRobot::setUnselected()
{
    pen.setColor(color);
    pen.setWidth(DEFAULT_PEN_WIDTH);
}
