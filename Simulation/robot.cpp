#include "robot.h"

Robot::Robot(double x, double y, double radius, double rot, double detRadius) : Circle::Circle(x, y, radius, rot)
{
    this->detRadius = detRadius;

    this->colliderFwd = Rectangle(x + detRadius/2, y, detRadius, 2*radius, rot);

    this->rotate(0);
}

Robot::~Robot() {}

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

bool Robot::obstacleDetection(std::vector<Obstacle*>* validObstacles)
{
    Circle* thisCircle = dynamic_cast<Circle*>(this);

    // Go through list of all other objects
    for(unsigned i = 0; i < validObstacles->size(); i++)
    {
        // Store current other object
        Rectangle* other = validObstacles->at(i)->getSimulationRectangle();

        Circle* c = dynamic_cast<Circle*>(other);
        // First check if robot and obstacles radiuses intersect
        if(Circle::intersect(thisCircle, c))
        {
            if(this->colliderFwd.intersects(other))
            {   
                return true;
            }
        }
    }

    return false;
}


bool Robot::obstacleDetection(std::vector<Rectangle*>* validObstacles)
{
    Circle* thisCircle = dynamic_cast<Circle*>(this);

    // Go through list of all other objects
    for(unsigned i = 0; i < validObstacles->size(); i++)
    {
        // Store current other object
        Rectangle* other = validObstacles->at(i);

        Circle* c = dynamic_cast<Circle*>(other);
        // First check if robot and obstacles radiuses intersect
        if(Circle::intersect(thisCircle, c))
        {
            if(this->colliderFwd.intersects(other))
            {
                return true;
            }
        }
    }

    return false;
}
