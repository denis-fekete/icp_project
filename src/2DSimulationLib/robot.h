#ifndef ROBOT_H
#define ROBOT_H

#include <vector>
#include <memory>

#include "rectangle.h"
#include "circle.h"
#include "../QtSpecific/obstacle.h"

class Robot : public Circle
{
    /**
     * @brief detRadius Radius in which will obstacles be detected, used for
     * less precise collision detection
     */
    double detRadius;

    /**
     * @brief colliderFwd Forward facing Rectangle for more detailed
     * collision detection
     */
    Rectangle colliderFwd;

    /**
     * @brief colliderInner Collider for other robots to know that they are too close
     * to this robot
     */
    Rectangle colliderInner;
public:

    /**
     * @brief Constructor of Robot object
     * @param x Center X position to be set
     * @param y Center Y position to be set
     * @param radius Radius if this Robot
     * @param rot Rotation of this robot
     * @param detRadius Detection radius of this robot
     */
    Robot(double x, double y, double radius, double rot, double detRadius);

    /**
     * @brief Checks if Robots collides with any Obstacle from vector of obstacles
     * @param validObstacles Vector of obstacles that Robot can collide with
     * @return True if Robot collides with one of the Obstacles
     */
    bool obstacleDetection(std::vector<std::unique_ptr<Obstacle>>* obstacles);

    /**
     * @brief Checks if Robots collides with any Rectangle from vector of obstacles
     * @param validObstacles Vector of obstacles that Robot can collide with
     * @return True if Robot collides with one of the Rectangle
     */
    bool obstacleDetection(std::vector<Rectangle*>* validObstacles);

    /**
     * @brief Moves this Robot forward in direction it is facing (based on `rotation`)
     * @param distance Value to move in forward facing direction
     */
    void moveForward(double distance);


    /**
     * @brief moveTo Moves Robot to Point `p`
     * @param p New center position of Robot
     */
    void moveTo(Point p);

    /**
     * @brief Add rotation to this Rectangle, and update all points
     * @param angle Angle to be added to the Rectangle
     */
    void rotate(double angle);

    /**
     * @return Returns this detection radius of this Robot
     */
    inline double getDetRadius() const { return this->detRadius; }

    /**
     * @return Returns pointer to forward facing rectangle
     */
    inline Rectangle* getColliderFwd() { return &(this->colliderFwd); }

    /**
     * @return Return pointer to inner rectangle
     */
    inline Rectangle* getColliderInner() { return &(this->colliderInner); }

    /**
     * @brief Checks if this Robot intersects with other Circle
     * @param other Pointer to the other Circle
     * @return True if this Robot collides with Circle
     */
    bool intersect(Circle* other);
};
#endif
