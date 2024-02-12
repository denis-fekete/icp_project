#ifndef ROBOT_H
#define ROBOT_H

#include <vector>
#include "rectangle.h"
#include "circle.h"

class Robot : public Circle
{
public:
    // Value of radius for collision detection
    double detRadius;
    //TODO: Speed at which will robot move
    double speed;
    Rectangle colliderFwd;

    //
    Robot(double x, double y, double radius, double rot, double detRadius);
    ~Robot();

    //
    void OnDetectedObstacle();
    bool ObstacleDetection(std::vector<Rectangle> validObstacles);

    void MoveForward(double distance);
    void Rotate(double angle);
};
#endif
