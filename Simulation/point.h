#ifndef POINT_2D_H
#define POINT_2D_H

#define MY_PI 3.14159265358979323846264338327950288419716939937510

#include <QDebug>

class Point
{
public:
    double x; // Position on X-axis
    double y; // Position on Y-axis

    Point(double x = 0, double y = 0);
    ~Point();

    inline void setPos(Point p)
    {
        this->x = p.x;
        this->y = p.y;
    }

    inline void setPos(double x, double y)
    {
        this->x = x;
        this->y = y;
    }

    inline Point getPos() { return *this;     }
    inline double getX() { return x; }
    inline double getY() { return y; }

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
