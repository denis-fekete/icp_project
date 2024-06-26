/**
 * @file manualrobot.h
 * @brief Header file for ManualRobot class. 
 * 
 * Manual robot is specialization 
 * of BaseRobot that performs/executes only orders from user provided through 
 * GUI.
 *
 * @author Denis Fekete (xfeket01@fit.vutbr.cz)
 */

#ifndef MANUALROBOT_H
#define MANUALROBOT_H


#include <QObject>
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QGraphicsScene>

#include <QTime>
#include <QDebug>

#include <vector>

#include "baserobot.h"

enum Command {STAY, ROTATE_ANTICLOCK, ROTATE_CLOCK, FORWARD};

class ManualRobot : public BaseRobot
{
protected:
    /**
     * @brief cmd Command that robot will execute
     */
    Command cmd;

public:
    /**
     * @brief Constructor of ManualRobot object
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
    ManualRobot(double x, double y, double radius, double rot,
              double detRadius, QColor color,
              std::vector<Rectangle*>* colliders,
              std::vector<Robot*>* robotColliders,
              Simulator* simulator, double* spaceWidth, double* spaceHeight,
              double* smoothConst);


    /**
     * @brief Simulates robot once, check if collision is occurring, if not
     * movesForward, if yes, rotates
     */
    void simulate() override;

    /**
     * @brief Method for drawing the robot
     */
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                       QWidget *widget) override;

    /**
     * @brief Set new Command for robot to execute
     * @param newCmd New Command to execute
     */
    inline void setCommand(Command newCmd) { this->cmd = newCmd; }

    /**
     * @brief Sets speed of this ManualRobot
     * @param speed New value to be set
     */
    inline void setSpeed(double speed) { this->speed = speed; }

    /**
     * @brief Sets turn angle of this ManualRobot
     * @param turnSpeed New value to be set
     */
    inline void setturnSpeed(double turnSpeed) { this->turnSpeed = turnSpeed; }

};

#endif // MANUALROBOT_H
