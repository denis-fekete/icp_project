#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <QGraphicsRectItem>
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QGraphicsScene>

#include <memory>

#include "../2DSimulationLib/rectangle.h"

class Obstacle : public QGraphicsItem
{
public:
    Obstacle(double x, double y, double w, double h, double rot, QColor& color, Obstacle** activeObstacle);

    void initialize(QGraphicsScene& scene);
    static void addObstacleToWorld(double x, double y, double w, double h, double rot,
                                   QColor& color, std::vector<std::unique_ptr<Obstacle>>& obstacles,
                                   QGraphicsScene& scene, Obstacle** activeObstacle);

    void rotateObstacle(double angle);
    inline Rectangle* getSimulationRectangle() { return &this->sim; };

// Qt
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

//getters
    inline Rectangle& getSim() { return this->sim; }
    inline QColor& getColor() { return this->color; }

    void setSelected();
    void setUnselected();
private:
    Obstacle** activeObstacle;
    Rectangle sim;
    QColor color;
    QBrush brush;
};

#endif // OBSTACLE_H
