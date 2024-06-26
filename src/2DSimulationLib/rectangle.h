/**
 * @file rectangle.h
 * @brief Header file for Rectangle class.
 * 
 * Rectangle is class for simulating 2D 4-edged convex polygon with parallel 
 * edges, which are same length ... a rotated rectangle. Class defines its 
 * size, position and rotations, also algorithms for colliding with other 
 * rectangles.
 *
 * @author Denis Fekete (xfeket01@fit.vutbr.cz)
 */

#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <QDebug>
#include <QtDebug>

#include "point.h"
#include "circle.h"


class Rectangle : public Circle
{
public:
    /**
     * @brief w Width of this Rectangle
     */
    double w;

    /**
     * @brief h Height of this Rectangle
     */
    double h;

    /**
     * @brief LB Left bottom Point
     */
    Point LB;

    /**
     * @brief LT Left top Point
     */
    Point LT;

    /**
     * @brief RB Right bottom Point
     */
    Point RB;

    /**
     * @brief RT Right top Point
     */
    Point RT;

    /**
     * @brief Constructor of Rectangle object
     */
    Rectangle();

    /**
     * @brief Constructor of Rectangle object
     * @param x X position to be set
     * @param y Y position to be set
     * @param w Width of the Rectangle
     * @param h Height of the Rectangle
     * @param rot Rotation of rectangle
     */
    Rectangle(double x, double y, double w, double h, double rot);
    ~Rectangle();

    /**
     * @brief Updates all points in rectangle based on rotation, offsets will
     * be respected
     */
    void updatePoints();

    /**
     * @brief Updates all points in rectangle based on rotation, offsets will
     * be respected
     * @param cosRad Calculates points based on `cosRad` value, if it was
     * already calculated
     * @param sinRad Calculates points based on `sinRad` value, if it was
     * already calculated
     */
    void updatePoints(double cosRad, double sinRad);

    /**
     * @brief Moves Rectangle to this Point
     * @param p New position of the center of the Rectangle
     */
    void moveTo(Point p);

    /**
     * @brief Moves this Rectangle in direction it is facing (based on rotation)
     * @param distance Distance to travel in forward direction
     */
    void moveForward(double distance);

    /**
     * @brief Add rotation to this Rectangle, and update all points
     * @param angle Angle to be added to the Rectangle
     */
    void rotate(double angle);

    /**
     * @brief Checks if this Rectangle intersects with `other` rectangle
     * @param other Reference to the other Rectangle
     * @return True if Rectangles intersect
     */
    bool intersects(Rectangle* other);

    /**
     * @return Returns width of this Rectangle
     */
    inline double getW() { return w; }

    /**
     * @return Returns height of this Rectangle
     */
    inline double getH() { return h; }

    /**
     * @brief Sets width of this Rectangle
     * @param w New value to be set
     */
    inline void setW(double w) { this->w = w; }

    /**
     * @brief Sets height of this Rectangle
     * @param h New value to be set
     */
    inline void setH(double h) { this->h = h; }

    /**
     * @brief Moves all points in rectangle in given deltas
     * @param deltaX Value to move in x axis
     * @param deltaY Value to move in y axis
     */
    inline void moveInDirection(double deltaX, double deltaY)
    {
        x += deltaX; y += deltaY;
        LB.x += deltaX; LB.y += deltaY;
        RB.x += deltaX; RB.y += deltaY;
        RT.x += deltaX; RT.y += deltaY;
        LT.x += deltaX; LT.y += deltaY;
    }

    /**
     * @brief Returns one of the Rectangle lines based on `line`
     * @param line Value based on which line of the Rectangle will be returned
     * @param start Start point of the line
     * @param end End point of the line
     */
    void breakIntoEdges(int line, Point* start, Point* end);


    /**
     * @brief pointInRectangle Returns whenever point lies inside this rectangle
     * @param p Point to be chcked
     */
    bool pointInRectangle(Point p);

protected:
    /**
     * @brief Updates single point based on `center` Point
     * @param center Point based on which position will be calculated
     * @param p Pointer to the Point that will be set
     * @param cosRad Cosine value based on which calculation will be done
     * @param sinRad Sine value based on which calculation will be done
     */
    static void updateSinglePoint(Point* center , Point* p, double cosRad, double sinRad);

    /**
     * @brief Calculates corners of `rect` Rectangle with no rotation
     * @param rect Rectangle whose `rect` will be changed
     */
    void calculateCornersWithNoRotation();
};

#endif
