/**
 * @file obstacle.h
 * @brief Header file for Obstacle class. 
 * 
 * Obstacle is an visual representation 
 * of Rectangles in GUI. It simulates convex polygon with 4 edges ... rotated 
 * rectangle.
 *
 * @author Denis Fekete (xfeket01@fit.vutbr.cz)
 */

#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <QGraphicsItem>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPen>

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
    QColor highlightedColor;

public:
    /**
     * @brief Constructor of Obstacle object
     * @param x Center X position to be set
     * @param y Center Y position to be set
     * @param w Width of the Obstacle
     * @param h Height of the Obstacle
     * @param rot Rotation of this robot
     * @param detRadius Detection radius of this robot
     * @param color Color of the AutoRobot
     * @param colliders Pointer to vector of Rectangle that will collide with this robot
     */
    Obstacle(double x, double y, double w, double h, double rot, QColor& color, Simulator* simulator);

   /**
     * @brief Initializes Obstacle values, this needs to called after constructor 
     */
    void initialize();

    /**
     * @brief Rotates Obstacle in simulation and graphics view
     * @param angle Value by which will Obstacle be rotated 
     */
    void rotateObstacle(double angle);
    
    /**
     * @return Rectangle* Returns pointer to the Rectangle that is used for simulation
     */
    inline Rectangle* getSimulationRectangle() { return &this->sim; };

    /**
     * @brief Method defining rough collider of the object for Qt
     * @return
     */
    QRectF boundingRect() const override;

    /**
     * @brief Method defining detailed collider of the object for Qt
     */
    QPainterPath shape() const override;

    /**
     * @brief Method for drawing the obstacle on the screen
     */
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

    /**
     * @return Rectangle* Returns pointer to simulation Rectangle object
     */
    inline Rectangle* getSim() { return &this->sim; }

    /**
     * @return QColor& Returns reference to color of this object 
     */
    inline QColor& getColor() { return this->color; }

    /**
     * @brief Updates values of this Obstacle
     * @param x New x position
     * @param y New y position
     * @param w New width
     * @param h New height
     * @param rot New rotation
     * @param color New color
     */
    void updateValues(double x, double y, double w, double h,
                                double rot, QColor color);

    /**
     * @brief Sets this robot to be highlighted
     */
    void setSelected();

      /**
     * @brief Un-sets this obstacle, to not be highlighted
     */
    void setUnselected();

};

#endif // OBSTACLE_H
