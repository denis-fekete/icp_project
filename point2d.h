#ifndef POINT_2D_H
#define POINT_2D_H

#include <cmath>

#define M_PI 3.14159265358979

class Point
{
private:
    double cosRad;
    double sinRad;

public:
    double x; // Position on X-axis
    double y; // Position on Y-axis
    double rot;

    Point(double x = 0, double y = 0, double rot = 0);
    ~Point();


    double GetCosRad();
    double GetSinRad();
    
    void SetCosRad(double val);
    void SetSinRad(double val);

    void CalculateSinCos();


    Point operator*(Point const& point)
    {
        Point res;
        res.x = x * point.x;
        res.y = y * point.y;

        return res;
    }

    Point operator+(Point const& point)
    {
        Point res;
        res.x = x + point.x;
        res.y = y + point.y;

        return res;
    }
};

#endif
