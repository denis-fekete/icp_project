#include "obstacle.h"
#include "qpen.h"

Obstacle::Obstacle(double x, double y, double w, double h, double rot, QColor& color, Obstacle** activeObstacle) : sim(x, y, w, h, rot)
{
    this->color = color;
    this->activeObstacle = activeObstacle;
}

void Obstacle::initialize()
{
    this->setPos(sim.getX(), sim.getY());
    this->setRotation(sim.getRotation());
    this->setFlag(QGraphicsItem::ItemSendsGeometryChanges);
    this->setFlag(QGraphicsItem::ItemIsMovable);
    this->setFlag(QGraphicsItem::ItemIsSelectable);

    this->setTransformOriginPoint(0, 0);
    brush = QBrush(color);
}

void Obstacle::rotateObstacle(double angle)
{
    this->sim.rotate(angle);
    this->setRotation(angle);
}

QRectF Obstacle::boundingRect() const
{
    return QRect(   -sim.w,
                    -sim.h,
                    2*sim.w,
                    2*sim.h);
}


QPainterPath Obstacle::shape() const
{
    QPainterPath path;
    path.addRect(-sim.w/2, -sim.h/2, sim.w, sim.h);

    return path;
}

void Obstacle::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->setBrush(brush);

    painter->drawRect(-sim.w/2, -sim.h/2, sim.w, sim.h);

    painter->setBrush(QBrush(this->color, Qt::BrushStyle::NoBrush));
}

QVariant Obstacle::itemChange(GraphicsItemChange change, const QVariant &value)
{

    if(change == GraphicsItemChange::ItemPositionHasChanged)
    {
        QPointF newPosition = value.toPointF();
        Point p(newPosition.x(), newPosition.y());
        this->sim.moveTo(p);
        *activeObstacle = this;
        return newPosition;
    }

    return QGraphicsItem::itemChange(change, value);
}

void Obstacle::setSelected()
{
    brush.setStyle(Qt::Dense1Pattern);
}

void Obstacle::setUnselected()
{
    brush.setStyle(Qt::SolidPattern);

}
