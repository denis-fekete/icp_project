#ifndef CIRCLE_H
#define CIRCLE_H

#include <math.h>
#include <cmath>
#include <QtMath>

#include "point2d.h"

class Circle : public Point
{
public:
    double rot;
    double radius;

    Circle();
    Circle(double x, double y, double radius, double rot);
    ~Circle();
    void MoveForward(double distance);
    void MoveTo(Point p);
    void Rotate(double angle);
};

#endif
