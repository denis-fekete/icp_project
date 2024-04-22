#include "line.h"

Line::Line(Point start, Point end)
{
    this->start.x = start.x;
    this->start.y = start.y;
    this->end.x = end.x;
    this->end.y = end.y;
}

Line::Line(double startX, double startY, double endX, double endY)
{
    this->start.x = startX;
    this->start.y = startY;
    this->end.x = endX;
    this->end.y = endY;
}

bool Line::intersectsWith(Line &other, Point& intersectionPoint)
{
    return Line::linesIntersects(*this, other, intersectionPoint);
}

bool Line::linesIntersects(Line& a, Line& b, Point& intersectionPoint)
{
    double denominator = (  (b.end.y - b.start.y) * (a.end.x - a.start.x)
                          - (b.end.x - b.start.x) * (a.end.y - a.start.y)  );

    // Lines are parallel
    if(denominator == 0)
    {
        return false;
    }

    double ua = (   (b.end.x - b.start.x) * (a.start.y - b.start.y)
                 -   (b.end.y - b.start.y) * (a.start.x - b.start.x))
                / denominator;
    double ub = (   (a.end.x - a.start.x) * (a.start.y - b.start.y)
                 -   (a.end.y - a.start.y) * (a.start.x - b.start.x))
                / denominator;

    // is the intersection along the segments
    if(ua < 0 || ua > 1 || ub < 0 || ub > 1)
    {
        return false;
    }

    intersectionPoint.x = a.start.x + ua * (a.end.x - a.start.x);
    intersectionPoint.y = a.start.y + ua * (a.end.y - a.start.y);

    return true;
}


bool Line::pointOnLeftSide(Point& p)
{
    auto res = (this->end.x - this->start.x) * (p.y - this->start.y) - (p.x - this->start.x) * (this->end.y - this->start.y);

    // If res >= 0 point is on the line, or on the left side
    return (res >= 0) ? true : false;
}


bool Line::pointOnLeftSide(Point& p, Point& lineStart, Point& lineEnd)
{
    auto res = (lineEnd.x - lineStart.x) * (p.y - lineStart.y) - (p.x - lineStart.x) * (lineEnd.y - lineStart.y);

    // If res >= 0 point is on the line, or on the left side
    return (res >= 0) ? true : false;
}

bool Line::pointOnLeftSide(Point& p, Line& line)
{
    auto res = (line.end.x - line.start.x) * (p.y - line.start.y) - (p.x - line.start.x) * (line.end.y - line.start.y);

    // If res >= 0 point is on the line, or on the left side
    return (res >= 0) ? true : false;
}
