#include "obstacle.h"
#include "qpen.h"

Obstacle::Obstacle(double x, double y, double w, double h, double rot, QColor& color) : sim(x, y, w, h, rot)
{
    this->color = color;

    this->setPos(sim.getX(), sim.getY());
    this->setRotation(sim.getRotation());
    this->setFlag(QGraphicsItem::ItemIsMovable);
    this->setFlag(QGraphicsItem::ItemSendsGeometryChanges);
}

void Obstacle::initialize(QGraphicsScene& scene)
{
    // this->setTransformOriginPoint(sim.x, sim.y);
    this->setTransformOriginPoint(0, 0);
    scene.addItem(this);
}

void Obstacle::rotateObstacle(double angle)
{
    this->sim.rotate(angle);
    this->setRotation(angle);
}

QRectF Obstacle::boundingRect() const
{
    // return QRect(   -sim.w/2,
    //                 -sim.h/2,
    //                 sim.w,
    //                 sim.h);

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
    QBrush brush(color);
    painter->setBrush(brush);

    painter->drawRect(-sim.w/2, -sim.h/2, sim.w, sim.h);

    painter->setBrush(QBrush(this->color, Qt::BrushStyle::NoBrush));
    painter->drawEllipse(-sim.getRadius(), -sim.getRadius(), 2 * sim.getRadius(), 2 * sim.getRadius());
}

QVariant Obstacle::itemChange(GraphicsItemChange change, const QVariant &value)
{

    if(change == GraphicsItemChange::ItemPositionHasChanged)
    {
        QPointF newPosition = value.toPointF();
        Point p(newPosition.x(), newPosition.y());
        this->sim.moveTo(p);

        return newPosition;
    }

    return QGraphicsItem::itemChange(change, value);
}


void Obstacle::addObstacleToWorld(double x, double y, double w, double h, double rot, QColor& color, std::vector<std::unique_ptr<Obstacle>>& obstacles, QGraphicsScene& scene)
{
    obstacles.push_back(std::make_unique<Obstacle> ( x, y, w, h, rot, color));

    obstacles.back()->initialize(scene);
}
