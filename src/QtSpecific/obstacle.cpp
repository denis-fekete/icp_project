/**
 * @file obstacle.cpp
 * @brief Implementation of Obstacle class methods
 *
 * @author Denis Fekete (xfeket01@fit.vutbr.cz)
 */

#include "obstacle.h"
#include "simulator.h"

Obstacle::Obstacle(double x, double y, double w, double h,
                   double rot, QColor& color, Simulator* simulator) :
    simulator(simulator), sim(x, y, w, h, rot), color(color)
{}

void Obstacle::initialize()
{
    this->rotateObstacle(0);

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
    // anti-aliasing
    this->setTransformationMode(Qt::SmoothTransformation);
    // make this appear on top of all objects, robots have 1
    this->setZValue(2);

    pen.setColor(color);

    highlightedColor.setRed(std::min(color.red() + 30, 240));
    highlightedColor.setGreen(std::min(color.green() + 30, 240));
    highlightedColor.setBlue(std::min(color.blue() + 30, 240));
}

void Obstacle::rotateObstacle(double angle)
{
    this->sim.rotate(angle);
    this->setRotation(sim.getRotation());
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
}

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
}


void Obstacle::setSelected()
{
    pen.setColor(highlightedColor);
    pen.setWidth(HIGHLIGHTED_PEN_WIDTH);
}

void Obstacle::setUnselected()
{
    pen.setColor(color);
    pen.setWidth(DEFAULT_PEN_WIDTH);
}

void Obstacle::updateValues(double x, double y, double w, double h,
                             double rot, QColor color)
{
    sim.setW(w);
    sim.setH(h);
    this->color = color;
    sim.setRotation(rot);
    // updates sinus and cosine values
    sim.calculateSinCos();

    // correctly rotate and move
    this->setPos(0, 0);
    this->setRotation(sim.getRotation());
    this->setPos(x, y);

    // updates point
    sim.updatePoints();
}
