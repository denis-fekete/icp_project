#include "point2d.h"

Point::~Point() {}

Point::Point(double x, double y, double rot)
{
    this->x = x;
    this->y = y;
    this->rot = rot;
}

void Point::CalculateSinCos()
{
    double radians = rot * (M_PI / 180.0);
    cosRad = cos(radians);
    sinRad = sin(radians);
}

void Point::CalculateSinCos(double rot)
{
    double radians = rot * (M_PI / 180.0);
    cosRad = cos(radians);
    sinRad = sin(radians);
}

void Point::CalculateSinCos(double* cosRad, double* sinRad, double* rot)
{
    double radians = this->rot * (M_PI / 180.0);
    this->cosRad = cos(radians);
    this->sinRad = sin(radians);

    *cosRad = this->cosRad;
    *sinRad = this->sinRad;
    *rot = this->rot;
}

double Point::GetCosRad()
{
    return cosRad;
}

double Point::GetSinRad()
{
    return sinRad;
}

void Point::SetCosRad(double val)
{
    this->cosRad = val;
}

void Point::SetSinRad(double val)
{
    this->sinRad = val;
}
