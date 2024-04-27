#ifndef BASEROBOT_H
#define BASEROBOT_H

#include <QObject>
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>

#include <QTime>
#include <QDebug>

#include <vector>

#include "../2DSimulationLib/robot.h"
#include "../2DSimulationLib/rectangle.h"

class Simulator;

#define DEFAULT_PEN_WIDTH 1
#define HIGHLIGHTED_PEN_WIDTH 3

class BaseRobot : public QGraphicsPixmapItem
{
protected:
    /**
     * @brief Robot object for simulation
     */
    Robot sim;
    /**
     * @brief Vector of all obstacles that robot can collide with
     */
    std::vector<Rectangle*>* colliders;

    /**
     * @brief Color of this robot
     */
    QColor color;

    /**
     * @brief Value whenever this robot was initalized
     */
    bool initialized;

    /**
     * @brief Pen for painting robot on scene
     */
    QPen pen;

    /**
     * @brief Color of this robot when highlighted
     */
    QColor highligtedColor;

    /**
     * @brief Pointer to the simulator
     */
    Simulator* simulator;
public:

    /**
     * @brief Constructor of AutoRobot object
     * @param x Center X position to be set
     * @param y Center Y position to be set
     * @param radius Radius if this Robot
     * @param rot Rotation of this robot
     * @param detRadius Detection radius of this robot
     * @param color Color of the AutoRobot
     * @param colliders Pointer to vector of Rectangle that will collide with this robot
     */
    BaseRobot(double x, double y, double radius, double rot,
              double detRadius, QColor color, std::vector<Rectangle*>* colliders, Simulator* simulator);
    /**
     * @brief Adds AutoRobot object to the scene
     * @param scene
     */
    void initialize();


    void setUnselected();
    void setSelected();

    /**
     * @brief Calls simulation move method for moving forward
     * @param distance Distance traveled
     */
    inline void moveRobot(double distance) { sim.moveForward(distance); }

    /**
     * @brief Calls simulation rotate method
     * @param angle Angle to be added to rotation
     */
    inline void rotateRobot(double angle) { sim.rotate(angle); }


    /**
     * @brief Simulates robot once, check if collision is occuring, if not
     * movesForward, if yes, rotates
     */
    virtual void simulate() = 0;

    /**
     * @brief Method defining rough collider of the object for Qt
     * @return
     */
    QRectF boundingRect() const override;

    /**
     * @brief Method defining detailded collider of the object for Qt
     */
    QPainterPath shape() const override;

    /**
     * @brief Method for drawing the robot
     */
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) = 0;

    /**
     * @brief Method for interactive moving AutoRobot in GUI
     */
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

    /**
     * @return Reference to simulation
     */
    inline Robot* getSim() { return &this->sim; }

    /**
     * @return Reference to Color
     */
    inline QColor& getColor() { return this->color; }

public slots:
    void positionUpdate();
};

#endif // BASEROBOT_H
