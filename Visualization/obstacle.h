#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <QGraphicsRectItem>
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QGraphicsScene>

#include <memory>

#include "../Simulation/rectangle.h"

class Obstacle : public QGraphicsItem
{
public:
    Obstacle(double x, double y, double w, double h, double rot, QColor& color);
    void initialize(QGraphicsScene& scene);
    static void addObstacleToWorld(double x, double y, double w, double h, double rot, QColor& color, std::vector<std::unique_ptr<Obstacle>>& obstacles, QGraphicsScene& scene);

    void rotateObstacle(double angle);
    inline Rectangle* getSimulationRectangle() { return &this->sim; };

// Qt
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

private:
    Rectangle sim;
    QColor color;
};

#endif // OBSTACLE_H
