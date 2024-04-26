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
     * @brief Speed of this robot
     */
    double speed;

    /**
     * @brief Turn angle on collision detection
     */

    double turnAngle;
    /**
     * @brief Direction in which will robot turn on collision
     */

    /**
     * @brief cmd Command that robot will execute
     */
    Command cmd;

public:
    /**
     * @brief Constructor of AutoRobot object
     * @param x Center X position to be set
     * @param y Center Y position to be set
     * @param radius Radius if this Robot
     * @param rot Rotation of this robot
     * @param detRadius Detection radius of this robot
     * @param color Color of the AutoRobot
     * @param speed Speed of the AutoRobot
     * @param obstaclesPointer Refernce to vector of obstacles
     */
    ManualRobot(double x, double y, double radius, double rot,
              double detRadius, QColor color,
              std::vector<Rectangle*>* colliders, Simulator* simulator);

    /**
     * @brief Simulates robot once, check if collision is occuring, if not
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
     * @param turnAngle New value to be set
     */
    inline void setTurnAngle(double turnAngle) { this->turnAngle = turnAngle; }

};

#endif // MANUALROBOT_H
