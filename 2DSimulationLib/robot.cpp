#include "robot.h"
#include <iostream>
#include <chrono>

Robot::Robot(double x, double y, double radius, double rot, double detRadius) : Circle::Circle(x, y, radius, rot), colliderFwd(x + detRadius/2, y, detRadius, radius, rot)
{
    this->detRadius = detRadius;
    this->rotate(0);
}

void Robot::moveForward(double distance)
{
    // TODO:
    // Circle::moveForward(distance);
    // colliderFwd.moveForward(distance);

    this->calculateSinCos (rot);

    // Calcualte delta value for moving in X and Y direction
    double xDelta = getCosRad() * distance;
    double yDelta = getSinRad() * distance;

    // Apply deltas to the current possition
    x += xDelta;
    y += yDelta;

    colliderFwd.x += xDelta;
    colliderFwd.y += yDelta;
    colliderFwd.LB.x += xDelta;
    colliderFwd.LB.y += yDelta;
    colliderFwd.RB.x += xDelta;
    colliderFwd.RB.y += yDelta;
    colliderFwd.RT.x += xDelta;
    colliderFwd.RT.y += yDelta;
    colliderFwd.LT.x += xDelta;
    colliderFwd.LT.y += yDelta;
}

double dAbs(double val)
{
    return (val > 0) ? val : -val;
}

void Robot::rotate(double angle)
{
    this->rot += angle;

    // Calculate whenever rotation is negative or positive number
    const int sign = (this->rot < 0)? -1 : 1;

    // Normalize rotation
    while(dAbs(this->rot) >= 360.0)
    {
        // Subtract 360 from rotation
        this->rot -= sign * 360;
    }

    this->calculateSinCos(this->rot);

    const double cosRadConst = this->getCosRad();
    const double sinRadConst = this->getSinRad();

    colliderFwd.setRotation(this->getRotation());

    // Calcualte delta value for moving in X and Y direction
    double xDelta = cosRadConst * this->detRadius / 2;
    double yDelta = sinRadConst * this->detRadius / 2;

    colliderFwd.x = this->x + xDelta;
    colliderFwd.y = this->y + yDelta;

    colliderFwd.updatePoints(cosRadConst, sinRadConst);
}

bool Robot::obstacleDetection(std::vector<std::unique_ptr<Obstacle>>& validObstacles)
{
    // Go through list of all other objects
    for(unsigned i = 0; i < validObstacles.size(); i++)
    {
        // Store current other object
        Rectangle* other = validObstacles.at(i)->getSimulationRectangle();

        Circle* otherCircle = dynamic_cast<Circle*>(other);
        // First check if robot and obstacles radiuses intersect
        if(this->intersect(otherCircle))
        {
            auto b = std::chrono::high_resolution_clock::now();
            std::cout << b.time_since_epoch().count() << std::endl;
            if(colliderFwd.intersects(other))
            {   
                return true;
            }
        }
    }

    return false;
}

bool Robot::obstacleDetection(std::vector<std::unique_ptr<Rectangle>>& validObstacles)
{
    // Go through list of all other objects
    for(unsigned i = 0; i < validObstacles.size(); i++)
    {
        // Store current other object
        Rectangle* other = validObstacles.at(i).get();

        Circle* otherCircle = dynamic_cast<Circle*>(other);
        // First check if robot and obstacles radiuses intersect
        if(this->intersect(otherCircle))
        {
            if(colliderFwd.intersects(other))
            {
                return true;
            }
        }
    }

    return false;
}

bool Robot::intersect(Circle* other)
{
    auto radius = (this->detRadius+ other->getRadius());
    radius = radius * radius;

    // Check if delta on X-axis between objects is less than detection radius + radius of other object
    if((this->x - other->getX()) * (this->x - other->getX()) <= radius)
    {
        // Check if delta on Y-axis between objects is less than detection radius + radius of other object
        if((this->y - other->getY()) * (this->y - other->getY()) <= radius)
        {
            return true;
        }
    }

    return false;
}
