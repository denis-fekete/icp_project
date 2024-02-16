#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <QGraphicsRectItem>
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QGraphicsScene>

#include "rectangle.h"

class Obstacle : public QGraphicsItem
{
public:
    Obstacle();
    Obstacle(double x, double y, double w, double h, double rot, QColor color);
    ~Obstacle();

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

    Rectangle* GetSimulationRectangle();

    void RotateObstacle(double angle);
    Rectangle* sim;
protected:
    QColor color;
};

#endif // OBSTACLE_H
