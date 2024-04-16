#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <QGraphicsRectItem>
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QGraphicsScene>

#include "../Simulation/rectangle.h"

class Obstacle : public QGraphicsItem
{
public:
    Obstacle();
    Obstacle(double x, double y, double w, double h, double rot, QColor& color);
    ~Obstacle();
    void initialize(QGraphicsScene& scene);
    static void addObstacleToWorld(double x, double y, double w, double h, double rot, QColor& color, std::vector<Obstacle*>& obstacles, QGraphicsScene& scene);

    void rotateObstacle(double angle);
    Rectangle* getSimulationRectangle();

    Rectangle* sim;
// Qt
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
protected:
    QColor color;
};

#endif // OBSTACLE_H
