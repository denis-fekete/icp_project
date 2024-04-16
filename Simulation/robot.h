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

    //
    Robot(double x, double y, double radius, double rot, double detRadius);
    ~Robot();

    //
    void onDetectedObstacle();
    bool obstacleDetection(std::vector<Obstacle*>* validObstacles);
    bool obstacleDetection(std::vector<Rectangle*>* validObstacles);

    void moveForward(double distance);
    void rotate(double angle);

    inline double getDetRadius() { return this->detRadius; }
};
#endif
