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
    short signed int turnDirection;
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
     * @param turnAngle Turn angle on collision detection
     * @param turnRight Turn direction
     * @param obstaclesPointer Pointer to the vector of obstacles
     */
    AutoRobot(double x, double y, double radius, double rot,
                        double detRadius, QColor color, double speed,
                        double turnAngle, bool turnRight,
                        std::vector<Rectangle*>* colliders,
                        std::vector<Robot*>* robotColliders,
                        Simulator* simulator);

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
    inline double getTurnAngle() { return this->turnAngle; }

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
