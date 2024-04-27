#include "obstacle.h"
#include "2DSimulationLib/simulator.h"
#include "qpen.h"

Obstacle::Obstacle(double x, double y, double w, double h,
                   double rot, QColor& color, Simulator* simulator) :
    simulator(simulator), sim(x, y, w, h, rot), color(color)
{}

void Obstacle::initialize()
{
    // set position and rotation to sim
    this->setTransformOriginPoint(0, 0);
    this->setRotation(sim.getRotation());
    this->setPos(sim.getX(), sim.getY());
    // sends signals when moved
    this->setFlag(QGraphicsItem::ItemSendsGeometryChanges);
    // can be moved
    this->setFlag(QGraphicsItem::ItemIsMovable);
    // can be selected, moved in group
    this->setFlag(QGraphicsItem::ItemIsSelectable);
    // antialising
    this->setTransformationMode(Qt::SmoothTransformation);
    // make this appear on top of all objects, obstacles have 1
    this->setZValue(1);

    highligtedColor.setRed(std::min(color.red() + 30, 240));
    highligtedColor.setGreen(std::min(color.green() + 30, 240));
    highligtedColor.setBlue(std::min(color.blue() + 30, 240));
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
    painter->setPen(pen);
    painter->setBrush(color);
    painter->drawRect(-sim.w/2, -sim.h/2, sim.w, sim.h);

    painter->setBrush(QBrush(this->color, Qt::BrushStyle::NoBrush));
}
/*
 * QVariant BaseRobot::itemChange(GraphicsItemChange change, const QVariant &value)
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
 */
QVariant Obstacle::itemChange(GraphicsItemChange change, const QVariant &value)
{
    auto flags = this->flags();
    if (change == GraphicsItemChange::ItemPositionHasChanged)
    {
        QPointF newPosition = value.toPointF();
        if (flags.testFlag(QGraphicsItem::ItemIsMovable))
        {
            Point p(newPosition.x(), newPosition.y());
            this->sim.moveTo(p);
            simulator->setActiveObstacle(this);
        }

        return newPosition;
    }
    else if (change == GraphicsItemChange::ItemSelectedChange)
    {
        simulator->setActiveObstacle(this);
    }

    return QGraphicsItem::itemChange(change, value);


    // if(change == GraphicsItemChange::ItemPositionHasChanged)
    // {
    //     QPointF newPosition = value.toPointF();
    //     Point p(newPosition.x(), newPosition.y());
    //     this->sim.moveTo(p);
    //     *activeObstacle = this;
    //     return newPosition;
    // }

    // return QGraphicsItem::itemChange(change, value);
}


void Obstacle::setSelected()
{
    pen.setColor(highligtedColor);
    pen.setWidth(HIGHLIGHTED_PEN_WIDTH);
}

void Obstacle::setUnselected()
{
    pen.setColor(color);
    pen.setWidth(DEFAULT_PEN_WIDTH);
}
