#ifndef ROBOT_H
#define ROBOT_H

#include <vector>

#include "rectangle.h"
#include "circle.h"
#include "../Visualization/obstacle.h"

class Robot : public Circle
{
public:
    // Value of radius for collision detection
    double detRadius;
    Rectangle colliderFwd;

    Robot(double x, double y, double radius, double rot, double detRadius);

    //
    void onDetectedObstacle();
    bool obstacleDetection(std::vector<std::unique_ptr<Obstacle>>& validObstacles);
    bool obstacleDetection(std::vector<std::unique_ptr<Rectangle>>& validObstacles);

    void moveForward(double distance);
    void rotate(double angle);

    inline double getDetRadius() const { return this->detRadius; }
};
#endif
