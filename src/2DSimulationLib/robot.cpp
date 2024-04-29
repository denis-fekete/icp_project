#include "robot.h"

Robot::Robot(double x, double y, double radius, double rot, double detRadius) :
    Circle::Circle(x, y, radius, rot),
    colliderFwd(x + detRadius / 2, y, detRadius, 2 * radius, rot)
{
    this->detRadius = detRadius;
}

void Robot::moveForward(double distance)
{
    this->calculateSinCos();

    // Calculate delta value for moving in X and Y direction
    double xDelta = getCosRad() * distance;
    double yDelta = getSinRad() * distance;

    // Apply deltas to the current position
    x += xDelta;
    y += yDelta;

    colliderFwd.moveInDirection(xDelta, yDelta);
}

void Robot::moveTo(Point p)
{
    this->x = p.x;
    this->y = p.y;

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

    // Fix colliderFwd position, rotate point around point
    calculateSinCos(rot);

    colliderFwd.setRotation(this->getRotation());

    // Reset collider position as if it was not rotated
    colliderFwd.x = x + detRadius / 2;
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

bool Robot::obstacleDetection(std::vector<Rectangle*>* validObstacles)
{
    // Go through list of all other objects
    for(unsigned i = 0; i < validObstacles->size(); i++)
    {
        // Store current other object
        Rectangle* other = validObstacles->at(i);

        Circle* otherCircle = dynamic_cast<Circle*>(other);
        // First check if robot and obstacles radiuses intersect
        if(this->intersect(*otherCircle))
        {
            if(colliderFwd.intersects(other))
            {
                return true;
            }
        }
    }

    return false;
}

bool Robot::intersect(Circle& other)
{
    auto left = (this->x - other.getX()) * (this->x - other.getX()) +
                (this->y - other.getY()) * (this->y - other.getY());

    return left <= ((this->detRadius + other.getRadius()) * (this->detRadius + other.getRadius()));
}

bool Robot::lineCircleIntersect(Point& lineStart, Point& lineEnd)
{

    double lineX1 = lineEnd.x - lineStart.x;
    double lineY1 = lineEnd.y - lineStart.y;

    double lineX2 = this->x - lineStart.x;
    double lineY2 = this->y - lineStart.y;

    double lineLength = lineX1 * lineX1 + lineY1 * lineY1;

    double t = std::max(0.0, std::min(lineLength, (lineX1 * lineX2 + lineY1 * lineY2))) / lineLength;

    double closestX = lineStart.x + t * lineX1;
    double closestY = lineStart.y + t * lineY1;

    double distance = sqrt((this->x - closestX) * (this->x - closestX) + (this->y - closestY) * (this->y - closestY));

    if(distance <= this->getRadius())
    {
        return true;
    }

    return false;
}


bool Robot::robotDetection(std::vector<Robot *> *robots)
{
    for(size_t i = 0; i < robots->size(); i++)
    {
        Robot* other = robots->at(i);
        if(other == this)
            continue;

        if(this->intersect(*other))
        {
            Point lineStart;
            Point lineEnd;
            for(int i = 0; i < 4; i++)
            {
                this->colliderFwd.breakIntoEdges(i, &lineStart, &lineEnd);

                if(other->lineCircleIntersect(lineStart, lineEnd))
                {
                    return true;
                }
            }
        }
    }

    return false;
}
