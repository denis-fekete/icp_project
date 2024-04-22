#ifndef CIRCLE_H
#define CIRCLE_H

#include <QtMath>

#include "point.h"

class Circle : public Point
{
protected:
    double rot;
    double radius;

    double cosRad;
    double sinRad;

public:
    Circle(double x, double y, double radius, double rot);

    void moveForward(double distance);
    void moveTo(Point p);
    void rotate(double angle);

    inline double getCosRad() const { return cosRad; }
    inline double getSinRad() const { return sinRad; }
    inline double getRadius() const { return radius; }
    inline double getRotation() const { return rot; }

    inline void setCosRad(double val) { this->cosRad = val; }
    inline void setSinRad(double val) { this->sinRad = val; }
    inline void setRotation(double rot) { this->rot = rot; }
    inline void setRadius(double rad) { this->radius = rad; }

    void calculateSinCos();
    void calculateSinCos(double rot);
    void calculateSinCos(double* cosRad, double* sinRad, double* rot);

    bool intersect(Circle* a, Circle* b)
    {
        auto radius = (a->radius + b->radius);
        radius = radius * radius;

        // Check if delta on X-axis between objects is less than detection radius + radius of other object
        if((a->x - b->x) * (a->x - b->x) <= radius)
        {
            // Check if delta on Y-axis between objects is less than detection radius + radius of other object
            if((a->y - b->y) * (a->y - b->y) <= radius)
            {
                return true;
            }
        }

        return false;
    }

    bool intersect(Circle& a, Circle& b)
    {
        auto radius = (a.radius + b.radius);
        radius = radius * radius;

        // Check if delta on X-axis between objects is less than detection radius + radius of other object
        if((a.x - b.x) * (a.x - b.x) <= radius)
        {
            // Check if delta on Y-axis between objects is less than detection radius + radius of other object
            if((a.y - b.y) * (a.y - b.y) <= radius)
            {
                return true;
            }
        }

        return false;
    }

};
#endif
