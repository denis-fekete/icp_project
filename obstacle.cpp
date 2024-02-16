#include "obstacle.h"
#include "qpen.h"


Obstacle::Obstacle() {}

Obstacle::~Obstacle() {
    delete this->sim;
}

Obstacle::Obstacle(double x, double y, double w, double h, double rot, QColor color)
{
    this->sim = new Rectangle(x, y, w, h, rot);

    this->color = color;

    this->setPos(this->sim->x, this->sim->y);
    this->setRotation(this->sim->rot);
    this->setFlag(QGraphicsItem::ItemIsMovable);
    this->setFlag(QGraphicsItem::ItemSendsGeometryChanges);
}

Rectangle* Obstacle::GetSimulationRectangle()
{
    return this->sim;
}

void Obstacle::RotateObstacle(double angle)
{
    this->sim->Rotate(angle);
    this->setRotation(angle);
}

QRectF Obstacle::boundingRect() const
{
    return QRect(-this->sim->w/2,
                -this->sim->h/2,
                this->sim->w,
                this->sim->h);
}


QPainterPath Obstacle::shape() const
{
    QPainterPath path;
    path.addRect(-this->sim->w/2, -this->sim->h/2, this->sim->w, this->sim->h);
    return path;
}

void Obstacle::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    QBrush brush(color);
    painter->setBrush(brush);

    painter->drawRect(- this->sim->w/2, - this->sim->h/2, this->sim->w, this->sim->h);
}

QVariant Obstacle::itemChange(GraphicsItemChange change, const QVariant &value)
{

    if(change == GraphicsItemChange::ItemPositionHasChanged)
    {
        QPointF newPosition = value.toPointF();
        Point p(newPosition.x(), newPosition.y());
        this->sim->MoveTo(p);

        return newPosition;
    }

    return QGraphicsItem::itemChange(change, value);
}
