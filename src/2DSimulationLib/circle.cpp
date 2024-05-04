/**
 * @file circle.cpp
 * @brief Implementation of Circle class methods
 *
 * @author Denis Fekete (xfeket01@fit.vutbr.cz)
 */

#include "circle.h"

Circle::Circle(double x, double y, double radius, double rot) : Point (x, y)
{
    // this->x = x;
    // this->y = y;
    this->radius = radius;
    this->rot = rot;
}

void Circle::moveTo(Point p)
{
    x = p.x;
    y = p.y;
}

void Circle::rotate(double angle)
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

    calculateSinCos();
}

void Circle::moveForward(double distance)
{
    // Calculate delta value for moving in X and Y direction
    double xDelta = getCosRad() * distance;
    double yDelta = getSinRad() * distance;

    // Apply deltas to the current position
    this->setPos(xDelta, yDelta);
}

void Circle::calculateSinCos()
{
    double radians = rot * (MY_PI / 180.0);
    cosRad = cos(radians);
    sinRad = sin(radians);
}

void Circle::calculateSinCos(double rot)
{
    double radians = rot * (MY_PI / 180.0);
    cosRad = cos(radians);
    sinRad = sin(radians);
}

void Circle::calculateSinCos(double* cosRad, double* sinRad, double* rot)
{
    double radians = this->rot * (MY_PI / 180.0);
    this->cosRad = cos(radians);
    this->sinRad = sin(radians);

    *cosRad = this->cosRad;
    *sinRad = this->sinRad;
    *rot = this->rot;
}
