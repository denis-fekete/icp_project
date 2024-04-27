#ifndef POINT_2D_H
#define POINT_2D_H

#define MY_PI 3.14159265358979323846264338327950288419716939937510

#include <QDebug>

class Point
{
public:
    /**
     * @brief x X Position of this Point
     */
    double x;
    /**
     * @brief y Y Position of this Point
     */
    double y;

    /**
     * @brief Constructor of Point object
     * @param x X position to be set, default value is 0
     * @param y Y Position to be set, default value is 0
     */
    Point(double x = 0, double y = 0);

    ~Point();

    /**
     * @brief Sets new value to this Point from input Point
     * @param p New value to be set
     */
    inline void setPos(Point p)
    {
        this->x = p.x;
        this->y = p.y;
    }

    /**
     * @brief Sets new value to this Point from two input double values
     * @param x New X position to be set
     * @param y New Y position to be set
     */
    inline void setPos(double x, double y)
    {
        this->x = x;
        this->y = y;
    }

    /**
     * @return Returns position of this point
     */
    inline Point getPos() const { return *this; }

    /**
     * @return Returns X Position of this Point
     */
    inline double getX() const { return x; }

    /**
     * @return Returns Y position of this point
     */
    inline double getY() const { return y; }

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
