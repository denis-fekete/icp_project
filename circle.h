#ifndef CIRCLE_H
#define CIRCLE_H

#include <QtMath>

#include "point2d.h"

class Circle : public Point
{
protected:
    double rot;
    double radius;

    double cosRad;
    double sinRad;

public:
    Circle();
    Circle(double x, double y, double radius, double rot);
    ~Circle();

    void MoveForward(double distance);
    void MoveTo(Point p);
    void Rotate(double angle);

    inline double GetCosRad() { return cosRad; }
    inline double GetSinRad() { return sinRad; }
    inline void SetCosRad(double val) { this->cosRad = val; }
    inline void SetSinRad(double val) { this->sinRad = val; }

    inline double GetRadius() { return this->radius; }
    inline double GetRotation() { return this->rot; }
    inline Point GetPosition() { return Point(this->x, this->y); }
    inline double GetXPos() { return this->x; }
    inline double GetYPos() { return this->y; }
    inline void SetRotation(double rot) { this->rot = rot; }
    inline void SetRadius(double rad) { this->radius = rad; }
    inline void SetXPos(double x) { this->x = x; }
    inline void SetYPos(double y) { this->y = y; }
    inline void SetPosition(Point p) { this->x = p.x; this->y = p.y; }

    void CalculateSinCos();
    void CalculateSinCos(double rot);
    void CalculateSinCos(double* cosRad, double* sinRad, double* rot);
};
#endif
