#include "baserobot.h"

BaseRobot::BaseRobot(double x, double y, double radius, double rot,
                     double detRadius, QColor color,
                     std::vector<Rectangle*>* colliders, BaseRobot **activeRobot) :
    sim(x, y, radius, rot, detRadius), colliders(colliders), color(color), activeRobot(activeRobot)
{
    initialized = false;
}

void BaseRobot::initialize()
{
    this->setFlag(QGraphicsItem::ItemIsMovable);
    this->setFlag(QGraphicsItem::ItemSendsGeometryChanges);
    this->setTransformOriginPoint(0, 0);
    initialized = true;
    this->brush = QBrush(color);
}

QRectF BaseRobot::boundingRect() const
{
    const double adj = 1;
    return QRectF(-sim.getRadius() - adj, -sim.getRadius() - adj, 2* sim.getRadius() + adj, 2*sim.getRadius() + adj);
}

QPainterPath BaseRobot::shape() const
{
    QPainterPath path;
    path.addEllipse(-sim.getRadius(), -sim.getRadius(),  sim.getRadius(), sim.getRadius());
    path.addRect(-sim.getDetRadius(), -sim.getDetRadius(), sim.getRadius(), sim.getRadius());
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
            *activeRobot = this;
        }

        return newPosition;
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
    brush.setStyle(Qt::Dense1Pattern);
}

void BaseRobot::setUnselected()
{
    brush.setStyle(Qt::SolidPattern);
}
