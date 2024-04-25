#ifndef AUTOROBOT_H
#define AUTOROBOT_H

#include <QObject>
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QGraphicsScene>

#include <QTime>
#include <QDebug>

#include <vector>

#include "obstacle.h"
#include "../2DSimulationLib/robot.h"

class AutoRobot : public QGraphicsItem
{
    // Q_OBJECT
private:
    /**
     * @brief Robot object for simulation
     */
    Robot sim;
    /**
     * @brief Vector of all obstacles that robot can collide with
     */
    std::vector<std::unique_ptr<Obstacle>>& obstacles;

    /**
     * @brief Color of this robot
     */
    QColor color;

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

    /**
     * @brief Value whenever this robot was initalized
     */
    bool initialized;

    /**
     * @brief Brush for paiting robot on scene
     */
    QBrush brush;

    AutoRobot** activeRobot;
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
    AutoRobot(double x, double y, double radius, double rot,
              double detRadius, QColor color, double speed,
              double turnAngle, bool turnRight,
              std::vector<std::unique_ptr<Obstacle>>& obstaclesPointer, AutoRobot** activeRobot);

    /*
     * @brief Creates and adds new AutoRobot to the scene
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
     * @param robots Reference to vector of AutoRobots
     * @param scene Scene to which AutoRobot will be added to
     */
    static void addRobotToWorld(double x, double y, double radius, double rot,
                                double detRadius, QColor color, double speed,
                                double turnAngle, bool turnRight,
                                std::vector<std::unique_ptr<Obstacle>>& obstaclesPointer,
                                std::vector<std::unique_ptr<AutoRobot>>& robots, QGraphicsScene& scene, AutoRobot** activeRobot);

    /**
     * @brief Adds AutoRobot object to the scene
     * @param scene
     */
    void initialize(QGraphicsScene& scene);



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
    void simulate();

    /**
     * @brief Method defining rough collider of the object for Qt
     * @return
     */
    QRectF boundingRect() const override;

    /**
     * @brief Method defining detailded collider of the object for Qt
     * @return
     */
    QPainterPath shape() const override;

    /**
     * @brief Method for drawing the robot
     * @param painter
     * @param option
     * @param widget
     */
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;


    /**
     * @return Reference to simulation
     */
    inline Robot& getSim() { return this->sim; }

    /**
     * @return Reference to Color
     */
    inline QColor& getColor() { return this->color; }

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

public slots:
    void positionUpdate();
};

#endif // AUTOROBOT_H
