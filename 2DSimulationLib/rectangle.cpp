#include "rectangle.h"

Rectangle::Rectangle(double x, double y, double w, double h, double rot) : Circle(x, y, sqrt( (w * w +  h * h) / 4), rot)
{
    this->w = w;
    this->h = h;
\
    updatePoints();
}

Rectangle::~Rectangle() {}

void Rectangle::moveTo(Point p)
{
    // Update center point
    this->x = p.x;
    this->y = p.y;

    // Call updatePoints to update points based on rotation
    this->updatePoints();
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
    // Transle back to original position with rotated position
    p->x = rotatedX + center->x;
    p->y = rotatedY + center->y;
}

void Rectangle::calculateCornersWithNoRotation(Rectangle* rect)
{
    // Calculate constants width/2 and heigth/2
    const auto w2 = rect->w / 2;
    const auto h2 = rect->h / 2;

    // Calculate corner points with no rotation
    rect->LB = Point(rect->x - w2, rect->y - h2);
    rect->LT = Point(rect->x - w2, rect->y + h2);
    rect->RB = Point(rect->x + w2, rect->y - h2);
    rect->RT = Point(rect->x + w2, rect->y + h2);
}

void Rectangle::updatePoints(double cosRad, double sinRad)
{
    calculateCornersWithNoRotation(this);
    Point center(this->x, this->y);

    // Update stored sinus and cosinus values
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
    calculateCornersWithNoRotation(this);

    Point center(this->x, this->y);

    // Update stored sinus and cosinus values
    this->calculateSinCos(this->rot);

    const double cosRad = this->getCosRad();
    const double sinRad = this->getSinRad();

    // Calculate corners with rotation
    updateSinglePoint(&center, &(this->LB), cosRad, sinRad);
    updateSinglePoint(&center, &(this->LT), cosRad, sinRad);
    updateSinglePoint(&center, &(this->RB), cosRad, sinRad);
    updateSinglePoint(&center, &(this->RT), cosRad, sinRad);
}

Line Rectangle::breakIntoEdges(RectLines line)
{
    switch (line)
    {
    case RectLines::bottom:
        return Line(this->LB, this->RB);
    case RectLines::right:
        return Line(this->RB, this->RT);
    case RectLines::top:
        return Line(this->RT, this->LT);
    default:
        return Line(this->LT, this->LB);
    }
}

bool Rectangle::pointInRectangle(Point& point)
{
    return (Line::pointOnLeftSide(point, this->LB, this->RB) &&
            Line::pointOnLeftSide(point, this->RB, this->RT) &&
            Line::pointOnLeftSide(point, this->RT, this->LT) &&
            Line::pointOnLeftSide(point, this->LT, this->LB));

    // if(Line::pointOnLeftSide(point, this->LB, this->RB))
    // {
    //     if(Line::pointOnLeftSide(point, this->RB, this->RT))
    //     {
    //         if(Line::pointOnLeftSide(point, this->RT, this->LT))
    //         {
    //             if(Line::pointOnLeftSide(point, this->LT, this->LB))
    //             {
    //                 return true;
    //             }
    //         }
    //     }
    // }

    // return false;
}

#define POLYGON_EDGE_COUNT 4
bool Rectangle::intersects(Rectangle* other)
{
    // Check points of rectangle
    // if(other->pointInRectangle(this->LB)) {return true; }
    // if(other->pointInRectangle(this->RB)) {return true; }
    // if(other->pointInRectangle(this->RT)) {return true; }
    // if(other->pointInRectangle(this->LT)) {return true; }
    // Check if other rectangle is not inside this rectaangle
    // if(pointInRectangle(other->LB)) {return true; }
    // if(pointInRectangle(other->RB)) {return true; }
    // if(pointInRectangle(other->RT)) {return true; }
    // if(pointInRectangle(other->LT)) {return true; }

    // Check whenever edges of rectangle intesect
    Point intersectionPoint;
    Line lineA{0, 0};
    Line lineB{0, 0};
    for(int i = 0; i < POLYGON_EDGE_COUNT; i++)
    {
        lineA = breakIntoEdges(static_cast<RectLines>(i));

        for (int j = 0; j < POLYGON_EDGE_COUNT; j++)
        {
            lineB = other->breakIntoEdges(static_cast<RectLines>(j));

            // Check if lines intersect
            if(Line::linesIntersects(lineA, lineB, intersectionPoint))
            {
                // Check if found point is in both rectangles
                if(this->pointInRectangle(intersectionPoint) && other->pointInRectangle(intersectionPoint))
                {
                    return true;
                }
            }
        }
    }

    return false;
}
#undef POLYGON_EDGE_COUNT
