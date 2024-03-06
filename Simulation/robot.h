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
    //TODO: Speed at which will robot move
    Rectangle colliderFwd;

    //
    Robot(double x, double y, double radius, double rot, double detRadius);
    ~Robot();

    //
    void OnDetectedObstacle();
    bool ObstacleDetection(std::vector<Obstacle*>* validObstacles);

    Point MoveForward(double distance);
    Point Rotate(double angle);
};
#endif
