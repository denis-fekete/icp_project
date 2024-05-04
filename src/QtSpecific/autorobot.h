/**
 * @file autorobot.h
 * @brief Header file for AutoRobot class. 
 * 
 * Automatic robot is specialization 
 * of BaseRobot that performs/executes actions based on its own algorithm.
 *
 * @author Denis Fekete (xfeket01@fit.vutbr.cz)
 */
#ifndef AUTOROBOT_H
#define AUTOROBOT_H

#include <QObject>
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QGraphicsScene>

#include <QTime>
#include <QDebug>

#include <vector>

#include "baserobot.h"


class AutoRobot : public BaseRobot
{
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
     * @param turnSpeed Turn angle on collision detection
     * @param turnDirection Turn direction, -1 or 1
     * @param colliders Pointer to the vector of obstacles
     * @param robotCollider Pointer to the vector of all robots
     * @param simulator Pointer to the simulator
     */
    AutoRobot(double x, double y, double radius, double rot,
              double detRadius, QColor color, double speed,
              double turnSpeed, short turnDirection,
              std::vector<Rectangle*>* colliders,
              std::vector<Robot*>* robotColliders,
              Simulator* simulator, double* spaceWidth, double* spaceHeight);


    /**
     * @brief Simulates robot once, check if collision is occurring, if not
     * movesForward, if yes, rotates
     */
    void simulate() override;

    /**
     * @return Speed of the AutoRobot
     */
    inline double getSpeed() { return this->speed; }

    /**
     * @return Turn angle of the AutoRobot
     */
    inline double getturnSpeed() { return this->turnSpeed; }

    /**
     * @return Turn direction of the AutoRobot
     */
    inline short signed int getTurnDirection() { return this->turnDirection; }

    /**
     * @brief Method for drawing the robot
     */
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                       QWidget *widget) override;
};

#endif // AUTOROBOT_H
