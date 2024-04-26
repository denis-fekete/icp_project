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

class ManualRobot : public BaseRobot
{
protected:

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
     * @param obstaclesPointer Refernce to vector of obstacles
     */
    ManualRobot(double x, double y, double radius, double rot,
              double detRadius, QColor color,
              std::vector<Rectangle*>* colliders, BaseRobot **activeRobot);

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
};

#endif // MANUALROBOT_H
