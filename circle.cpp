#include <circle.h>

Circle::Circle() {}

Circle::Circle(double x, double y, double radius, double rot) : Point (x, y, rot)
{
    // this->x = x;
    // this->y = y;
    this->radius = radius;
    this->rot = rot;
}

Circle::~Circle() {}

void Circle::MoveTo(Point p)
{
    x = p.x;
    y = p.y;
}

void Circle::Rotate(double angle)
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
}

void Circle::MoveForward(double distance)
{
    // function cos() and sin() from cmath takes radians
    double radians = rot * (M_PI / 180);

    // Calcualte delta value for moving in X and Y direction
    double xDelta = cos(radians) * distance;
    double yDelta = sin(radians) * distance;

    // Apply deltas to the current possition
    x += xDelta;
    y += yDelta;
}
