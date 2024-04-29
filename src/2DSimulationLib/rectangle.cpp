#include "rectangle.h"

Rectangle::Rectangle(double x, double y, double w, double h, double rot) : Circle(x, y, sqrt( (w * w +  h * h) / 4), rot)
{
    this->w = w;
    this->h = h;
}

Rectangle::~Rectangle() {}

void Rectangle::moveTo(Point p)
{
    // Update center point
    this->x = p.x;
    this->y = p.y;

    // Call updatePoints to update points based on rotation
    updatePoints(cosRad, sinRad);
}

void Rectangle::moveForward(double distance)
{
    // Calcualte delta value for moving in X and Y direction
    double xDelta = getCosRad() * distance;
    double yDelta = getSinRad() * distance;

    // Apply deltas to the current possition
    x += xDelta;
    y += yDelta;

    const Point p(xDelta, yDelta);

    LB = LB + p;
    RB = RB + p;
    RT = RT + p;
    LT = LT + p;
}

void Rectangle::rotate(double angle)
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

    // Update points / corners
    updatePoints();
}

void Rectangle::updateSinglePoint(Point* center , Point* p, double cosRad, double sinRad)
{
    // Calculate translated position
    double tmpX = p->x - center->x;
    double tmpY = p->y - center->y;
    // Calculate rotated position
    double rotatedX = (tmpX * cosRad) - (tmpY * sinRad);
    double rotatedY = (tmpX * sinRad) + (tmpY * cosRad);
    // Translate back to original position with rotated position
    p->x = rotatedX + center->x;
    p->y = rotatedY + center->y;
}

void Rectangle::calculateCornersWithNoRotation()
{
    // Calculate constants width/2 and height/2
    const auto w2 = this->w / 2;
    const auto h2 = this->h / 2;

    // Calculate corner points with no rotation
    this->LB.setPos(this->x - w2, this->y - h2);
    this->LT.setPos(this->x - w2, this->y + h2);
    this->RB.setPos(this->x + w2, this->y - h2);
    this->RT.setPos(this->x + w2, this->y + h2);

}

void Rectangle::updatePoints(double cosRad, double sinRad)
{
    calculateCornersWithNoRotation();
    Point center(this->x, this->y);

    // Update stored sinus and cosine values
    this->setCosRad(cosRad);
    this->setSinRad(sinRad);

    // Calculate corners with rotation
    updateSinglePoint(&center, &LB, cosRad, sinRad);
    updateSinglePoint(&center, &LT, cosRad, sinRad);
    updateSinglePoint(&center, &RB, cosRad, sinRad);
    updateSinglePoint(&center, &RT, cosRad, sinRad);
}

void Rectangle::updatePoints()
{
    calculateCornersWithNoRotation();

    Point center(this->x, this->y);

    // Update stored sinus and cosine values
    this->calculateSinCos(this->rot);

    const double cosRad = this->getCosRad();
    const double sinRad = this->getSinRad();

    // Calculate corners with rotation
    updateSinglePoint(&center, &(this->LB), cosRad, sinRad);
    updateSinglePoint(&center, &(this->LT), cosRad, sinRad);
    updateSinglePoint(&center, &(this->RB), cosRad, sinRad);
    updateSinglePoint(&center, &(this->RT), cosRad, sinRad);
}

bool Rectangle::onSegment(Point& a, Point& b, Point& c)
{
    return (b.x <= std::max(a.x, c.x) && b.x >= std::min(a.x, c.x) &&
            b.y <= std::max(a.y, c.y) && b.y >= std::min(a.y, c.y));
}


double Rectangle::orientation(Point& a, Point& b, Point& c)
{
    double val = (b.y - a.y) * (c.x - b.x) -
              (b.x - a.x) * (c.y - b.y);

    if (val >= -0.01 && val <= 0.01) return 0;

    return (val > 0)? 1: 2;
}

bool Rectangle::linesIntersect(Point& startA, Point& endA, Point& startB, Point& endB)
{
    double o1 = orientation(startA, endA, startB);
    double o2 = orientation(startA, endA, endB);
    double o3 = orientation(startB, endB, startA);
    double o4 = orientation(startB, endB, endA);

    if (o1 != o2 && o3 != o4)
        return true;

    // Special Cases
    // p1, q1 and p2 are collinear and p2 lies on segment p1q1
    if (o1 == 0 && onSegment(startA, endB, endA)) return true;

    // p1, q1 and q2 are collinear and q2 lies on segment p1q1
    if (o2 == 0 && onSegment(startA, endB, endA)) return true;

    // p2, q2 and p1 are collinear and p1 lies on segment p2q2
    if (o3 == 0 && onSegment(startB, startA, endB)) return true;

    // p2, q2 and q1 are collinear and q1 lies on segment p2q2
    if (o4 == 0 && onSegment(startB, startA, endB)) return true;

    return false;
}

#define POLYGON_EDGE_COUNT 4
#define LB_RB_LINE 0
#define RB_RT_LINE 1
#define RT_LT_LINE 2
#define LT_LB_LINE 3

void Rectangle::breakIntoEdges(int line, Point* start, Point* end)
{
    switch (line)
    {
    case LB_RB_LINE:
        *start = this->LB;
        *end = this->RB;
        break;
    case RB_RT_LINE:
        *start = this->RB;
        *end = this->RT;
        break;
    case RT_LT_LINE:
        *start = this->RT;
        *end = this->LT;
        break;
    default:
        *start = this->LT;
        *end = this->LB;
        break;
    }
}

bool Rectangle::intersects(Rectangle* other)
{
    // Check whenever edges of rectangle intesect
    Point startA, endA;
    Point startB, endB;
    for(int i = 0; i < POLYGON_EDGE_COUNT; i++)
    {
        this->breakIntoEdges(i, &startA, &endA);

        for (int j = 0; j < POLYGON_EDGE_COUNT; j++)
        {
            other->breakIntoEdges(j, &startB, &endB);

            if(linesIntersect(startA, endA, startB, endB))
            {
                return true;
            }
        }
    }

    return false;
}

#undef POLYGON_EDGE_COUNT
#undef POLYGON_EDGE_COUNT
#undef LB_RB_LINE
#undef RB_RT_LINE
#undef RT_LT_LINE
#undef LT_LB_LINE
