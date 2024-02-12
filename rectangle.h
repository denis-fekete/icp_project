#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <point2d.h>
#include <cmath>
#include <iostream>

class Rectangle : public Point
{
public:
    double w;
    double h;
    double rot;

    double radius;

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
};

#endif