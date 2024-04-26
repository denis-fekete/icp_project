#include "robot.h"

#include <iostream>


Robot::Robot(double x, double y, double radius, double rot, double detRadius) :
    Circle::Circle(x, y, radius, rot),
    // x + (rad + detRad) / 2 - rad = center of collider rectangle
    colliderFwd(x + (radius + detRadius)/2 - radius, y, radius + detRadius, 2 * radius, rot),
    colliderInner (x, y, radius, radius, 0)
{
    this->detRadius = detRadius;
    this->rotate(0);
}

void Robot::moveForward(double distance)
{
    this->calculateSinCos (rot);

    // Calcualte delta value for moving in X and Y direction
    double xDelta = getCosRad() * distance;
    double yDelta = getSinRad() * distance;

    // Apply deltas to the current possition
    x += xDelta;
    y += yDelta;

    colliderFwd.moveInDirection(xDelta, yDelta);
    colliderInner.moveInDirection(xDelta, yDelta);
}

void Robot::moveTo(Point p)
{
    this->x = p.x;
    this->y = p.y;

    colliderInner.moveTo(p);

    rotate(0);
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

    colliderFwd.setRotation(getRotation());

    // Fix colliderFwd position, rotate point around point
    calculateSinCos(rot);
    // Reset collider position as if it was not rotated
    colliderFwd.x = x + (radius + detRadius)/2 - radius;
    colliderFwd.y = y;
    // Translate back to origin
    colliderFwd.x -= this->x;
    colliderFwd.y -= this->y;
    // rotate it
    double xDelta = colliderFwd.x * cosRad - colliderFwd.y * sinRad;
    double yDelta = colliderFwd.x * sinRad + colliderFwd.y * cosRad;
    // translate point back
    colliderFwd.x = this->x + xDelta;
    colliderFwd.y = this->y + yDelta;

    colliderFwd.updatePoints(cosRad, sinRad);
}

bool Robot::obstacleDetection(std::vector<std::unique_ptr<Obstacle>>* validObstacles)
{
    // Go through list of all other objects
    for(unsigned i = 0; i < validObstacles->size(); i++)
    {
        // Store current other object
        Rectangle* other = validObstacles->at(i)->getSimulationRectangle();

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

bool Robot::obstacleDetection(std::vector<Rectangle*>* validObstacles)
{
    // Go through list of all other objects
    for(unsigned i = 0; i < validObstacles->size(); i++)
    {
        // Store current other object
        Rectangle* other = validObstacles->at(i);

        if(other == &(this->colliderInner))
            continue;

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
    auto radius = (this->detRadius + other->getRadius());
    auto left = (this->x - other->getX()) * (this->x - other->getX()) +
                (this->y - other->getY()) * (this->y - other->getY());
    auto right = radius * radius;
    if( left <= right)
    {
        return true;
    }

    return false;
}
