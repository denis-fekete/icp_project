#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <QDebug>
#include <QtDebug>

#include "point2d.h"
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

    void UpdatePoints();
    void UpdatePoints(double cosRad, double sinRad);

    void MoveTo(Point p);
    void MoveForward(double distance);
    void Rotate(double angle);

    bool Intersects(Rectangle* other);

protected:
    static void UpdateSinglePoint(Point* center , Point* p, double cosRad, double sinRad);
    static void CalcualteCornersWithNoRotation(Rectangle* rect);
    static bool LineIntersects(Point* startA, Point* endA, Point* startB, Point* endB, Point *intersectionPoint);
    static void BreakIntoEdges(Point* pStart, Point* pEnd, Rectangle* rect, unsigned edge);
    static bool OnLeftSideOfLine(Point S, Point A, Point B);
    static bool PointInRectangle(Point* point, Rectangle* rect);
};

#endif
