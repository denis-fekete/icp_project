#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <QGraphicsRectItem>
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QGraphicsScene>

#include "../2DSimulationLib/rectangle.h"

#define DEFAULT_PEN_WIDTH 1
#define HIGHLIGHTED_PEN_WIDTH 3
class Simulator;

class Obstacle : public QGraphicsPixmapItem
{
private:
    /**
     * @brief Pointer to the simulator
     */
    Simulator* simulator;
    /**
     * @brief Rectangle object for simulation
     */
    Rectangle sim;

    /**
     * @brief Color of this Obstacle
     */
    QColor color;

    /**
     * @brief Pen for painting Obstacle on scene
     */
    QPen pen;

    /**
     * @brief Color of this Obstacle when highlighted
     */
    QColor highligtedColor;

public:
    Obstacle(double x, double y, double w, double h, double rot, QColor& color, Simulator* simulator);

    void initialize();

    void rotateObstacle(double angle);
    inline Rectangle* getSimulationRectangle() { return &this->sim; };

// Qt
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

//getters
    inline Rectangle* getSim() { return &this->sim; }
    inline QColor& getColor() { return this->color; }

    void setSelected();
    void setUnselected();

};

#endif // OBSTACLE_H
