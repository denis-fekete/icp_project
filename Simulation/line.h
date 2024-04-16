#ifndef LINE_H
#define LINE_H

#include "point.h"
#include "QDebug"

class Line
{
public:
    Point start;
    Point end;

    Line(Point start, Point end);
    Line(double startX, double startY, double endX, double endY);

    bool intersectsWith(Line other, Point* intersectionPoint);
    static bool linesIntersects(Line a, Line b, Point* intersectionPoint);

    bool pointOnLeftSide(Point p);
    static bool pointOnLeftSide(Point p, Line line);
    static bool pointOnLeftSide(Point p, Point lineStart, Point lineEnd);
};

#endif // LINE_H
