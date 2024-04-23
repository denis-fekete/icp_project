#ifndef ROBOT_H
#define ROBOT_H

#include <vector>

#include "rectangle.h"
#include "circle.h"
#include "../QtSpecific/obstacle.h"

class Robot : public Circle
{
    /**
     * @brief detRadius Radius in which will obstacles be detected, used for
     * less precise collision detectiom
     */
    double detRadius;

    /**
     * @brief colliderFwd Forward facing Rectangle for more detailed
     * collision detection
     */
    Rectangle colliderFwd;
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
    bool obstacleDetection(std::vector<std::unique_ptr<Obstacle>>& validObstacles);

    /**
     * @brief Checks if Robots collides with any Rectangle from vector of obstacles
     * @param validObstacles Vector of obstacles that Robot can collide with
     * @return True if Robot collides with one of the Rectangle
     */
    bool obstacleDetection(std::vector<std::unique_ptr<Rectangle>>& validObstacles);

    /**
     * @brief Moves this Robot forward in direction it is facing (based on `rotation`)
     * @param distance Value to move in forward facing direction
     */
    void moveForward(double distance);

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
     * @brief Checks if this Robot intersects with other Circle
     * @param other Pointer to the other Circle
     * @return True if this Robot collides with Circle
     */
    bool intersect(Circle* other);
};
#endif
