#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <QDebug>
#include <QtDebug>

#include "point.h"
#include "line.h"
#include "circle.h"

class Rectangle : public Circle
{
public:
    double w;
    double h;

    Point LB;
    Point LT;
    Point RB;
    Point RT;

    Rectangle();
    Rectangle(double x, double y, double w, double h, double rot);
    ~Rectangle();

    void updatePoints();
    void updatePoints(double cosRad, double sinRad);

    void moveTo(Point p);
    void moveForward(double distance);
    void rotate(double angle);

    bool intersects(Rectangle* other);

protected:
    using RectLines = enum RectangleLines {bottom, left, top, right};
    static void updateSinglePoint(Point* center , Point* p, double cosRad, double sinRad);
    static void calculateCornersWithNoRotation(Rectangle* rect);
    // static bool lineIntersects(Point* startA, Point* endA, Point* startB, Point* endB, Point *intersectionPoint);
    // static void breakIntoEdges(Point* pStart, Point* pEnd, Rectangle* rect, unsigned edge);
    // static bool onLeftSideOfLine(Point S, Point A, Point B);
    Line breakIntoEdges(RectLines line);
    static bool pointInRectangle(Point* point, Rectangle* rect);
};

#endif
