#include <robot.h>

// HELPING FUNCTIONS
bool distanceBetweenTwoCircles(Point c1, double rad1, Point c2, double rad2);
//

Robot::Robot(double x, double y, double radius, double rot, double detRadius) : Circle::Circle(x, y, radius, rot)
{
    this->detRadius = detRadius;

    this->colliderFwd = Rectangle(x+radius+detRadius/2, y, detRadius, 2*radius, rot);

    Rotate(0);
}

Robot::~Robot() {}

bool distanceBetweenTwoCircles(Point c1, double rad1, Point c2, double rad2)
{
    auto radius = rad1 + rad2;

    // Check if delta on X-axis between objects is less than detection radius + radius of other object
    if((c1.x - c2.x) * (c1.x - c2.x) <= radius * radius)
    {
        // Check if delta on Y-axis between objects is less than detection radius + radius of other object
        if((c1.y - c1.y) * (c1.y - c2.y) <= radius * radius)
        {
            return true;
        }
    }
                
    return false;
}

void Robot::MoveForward(double distance)
{
    // Calcualte delta value for moving in X and Y direction
    double xDelta = this->GetCosRad() * distance;
    double yDelta = this->GetSinRad() * distance;

    // Apply deltas to the current possition
    x += xDelta;
    y += yDelta;

    // Apply movement to the collider
    const Point p(xDelta, yDelta);

    colliderFwd.x += xDelta;
    colliderFwd.y += yDelta;

    colliderFwd.LB = colliderFwd.LB + p;
    colliderFwd.RB = colliderFwd.RB + p;
    colliderFwd.RT = colliderFwd.RT + p;
    colliderFwd.LT = colliderFwd.LT + p;
}

void Robot::Rotate(double angle)
{
    rot += angle;

    // Calculate whenever rotation is negative or positive number
    const int sign = (rot < 0)? -1 : 1;

    // Normalize rotation
    while(abs(rot) >= 360.0)
    {
        // Subtract 360 from rotation
        rot -= sign * 360;
    }

    this->CalculateSinCos();

    const double cosRad = this->GetCosRad();
    const double sinRad = this->GetSinRad();
    
    colliderFwd.rot = this->rot;

    // Calcualte delta value for moving in X and Y direction
    double xDelta = cosRad * detRadius;
    double yDelta = sinRad * detRadius;

    colliderFwd.x = this->x + xDelta;
    colliderFwd.y = this->y + yDelta;

    colliderFwd.UpdatePoints(cosRad, sinRad);
}

bool Robot::ObstacleDetection(std::vector<Rectangle> validObstacles)
{
    // Go through list of all other objects 
    for(unsigned i = 0; i < validObstacles.size(); i++)
    {
        // Store current other object
        auto other = validObstacles[i];

        // First check if robot and obstacles radiuses intersect
        if(distanceBetweenTwoCircles(Point(this->x, this->y), this->detRadius, Point(other.x, other.y), other.rot))
        {

            if(this->colliderFwd.Intersects(&other))
            {
                return true;
            }
        }
    }

    return false;
}