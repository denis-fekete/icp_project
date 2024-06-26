/**
 * @file baserobot.h
 * @brief Header file for BaseRobot class. 
 * 
 * BaseRobot is class for graphical 
 * representation of Robots in Simulation. It contains basic attributes and 
 * methods that can be shared between its specializations like AutoRobot and 
 * ManualRobot.
 *
 * @author Denis Fekete (xfeket01@fit.vutbr.cz)
 */

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

// class BaseRobot : public QGraphicsPixmapItem
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
     * @brief Vector of all other robots colliders
     */
    std::vector<Robot*>* robotColliders;

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
    QColor highlightedColor;

    /**
     * @brief Pointer to the simulator
     */
    Simulator* simulator;


    /**
     * @brief Speed of this robot
     */
    double speed;

    /**
     * @brief Turn angle on collision detection
     */

    double turnSpeed;

    /**
     * @brief Direction in which will robot turn on collision
     */
    short turnDirection;

    /**
     * @brief spaceWidth Pointer to the width of simulation space
     */
    double* spaceWidth;

    /**
     * @brief spaceHeight Pointer to the height of simulation space
     */
    double* spaceHeight;

    /**
     * @brief smoothConst Constant calculated from update time of Simulator for
     * smoother movement
     */
    double* smoothConst;
public:

    /**
     * @brief Constructor of BaseRobot object
     * @param x Center X position to be set
     * @param y Center Y position to be set
     * @param radius Radius if this Robot
     * @param rot Rotation of this robot
     * @param detRadius Detection radius of this robot
     * @param color Color of the AutoRobot
     * @param speed Speed of the AutoRobot
     * @param turnSpeed Turn angle on collision detection
     * @param turnDirection Turn direction, -1 or 1
     * @param colliders Pointer to the vector of obstacles
     * @param robotCollider Pointer to the vector of all robots
     * @param simulator Pointer to the simulator
     * @param smoothConst Pointer to the smoothing constant calcualted by simulator
     */
    BaseRobot(double x, double y, double radius, double rot,
              double detRadius, QColor color, double speed,
              double turnSpeed, short turnDirection,
              std::vector<Rectangle*>* colliders,
              std::vector<Robot*>* robotColliders,
              Simulator* simulator, double* spaceWidth, double* spaceHeight,
              double* smoothConst);

    /**
     * @brief Initializes Robot values, this needs to called after constructor 
     */
    void initialize();

    /**
     * @brief Sets this robot to be highlighted
     */
    void setSelected();

    /**
     * @brief Un-sets this robot, to not be highlighted
     */
    void setUnselected();

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
     * @brief Simulates robot once, check if collision is occurring, if not
     * movesForward, if yes, rotates
     */
    virtual void simulate() = 0;

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
     * @brief Method for drawing the robot
     */
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) = 0;

    /**
     * @brief Method for interactive moving AutoRobot in GUI
     */
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

    /**
     * @return Robot* Returns pointer to the simulation Robot
     */
    inline Robot* getSim() { return &this->sim; }

    /**
     * @return Reference to Color
     */
    inline QColor& getColor() { return this->color; }

    /**
     * @return Returns speed speed of this robot
     */
    inline double getSpeed() { return this->speed; }


    /**
     * @return Returns speed speed of this robot
     */
    inline double getTurnSpeed() { return this->turnSpeed; }

    /**
     * @return Returns turn direction of this robot
     */
    inline int getTurnDirection() { return this->turnDirection; }

    /**
     * @brief Updates values of this robot
     * @param x New x position
     * @param y New y position
     * @param radius New radius
     * @param rot New rotation
     * @param detRadius New detection radius
     * @param color New color
     * @param speed New speed
     * @param turnSpeed New turning speed
     * @param turnDirection New turning direction
     */
    void updateValues(double x, double y, double radius, double rot,
                                 double detRadius, QColor color, double speed,
                                 double turnSpeed, short turnDirection);


    /**
     * @return Returns whenever robot is outside of simulated space
     */
    inline bool isOutsideSimulation()
    {
        return (sim.getColliderFwd()->RB.x > *spaceWidth ||
                sim.getColliderFwd()->RB.y > *spaceHeight ||
                sim.getColliderFwd()->RB.x < 0 ||
                sim.getColliderFwd()->RB.y < 0 ||

                sim.getColliderFwd()->RT.x > *spaceWidth ||
                sim.getColliderFwd()->RT.y > *spaceHeight ||
                sim.getColliderFwd()->RT.x < 0 ||
                sim.getColliderFwd()->RT.y < 0
                );
    }
protected:
    void advance(int step) Q_DECL_OVERRIDE;
public:
    void positionUpdate();
};

#endif // BASEROBOT_H
