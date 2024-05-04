/**
 * @file circle.h
 * @brief Header file for Circle class.
 * 
 * Circle represents circle object with radius positioned in 2D space, used 
 * for simulations.
 *
 * @author Denis Fekete (xfeket01@fit.vutbr.cz)
 */

#ifndef CIRCLE_H
#define CIRCLE_H

#include <QtMath>

#include "point.h"

class Circle : public Point
{
protected:
    /**
     * @brief rot Rotation of this Circle
     */
    double rot;
    /**
     * @brief radius Radius of this Circle
     */
    double radius;
    /**
     * @brief cosRad Stored cosine value for better performance
     * (less cos() function calls)
     */
    double cosRad;
    /**
     * @brief sinRad Stored sine value for better performance
     * (less cos() function calls)
     */
    double sinRad;

public:
    /**
     * @brief Circle constructor
     * @param x Center x position of the Circle
     * @param y Center y position of the Circle
     * @param radius Radius of the Circle
     * @param rot Rotation angle of the circle
     */
    Circle(double x, double y, double radius, double rot);

    /**
     * @brief Moves circle forward in direction it is facing, based on
     * stored `rotation`
     * @param distance Distance to travel in forward direction
     */
    void moveForward(double distance);
    /**
     * @brief Changes position of Circle to given point
     * @param p New center position of this Circle
     */
    void moveTo(Point p);
    /**
     * @brief Add rotation to Circle
     * @param angle Rotation to add to the Circle
     */
    void rotate(double angle);

    /**
     * @return Returns stored cosine
     */
    inline double getCosRad() const { return cosRad; }
    /**
     * @return Return stored sinus
     */
    inline double getSinRad() const { return sinRad; }
    /**
     * @return Return radius of Circle
     */
    inline double getRadius() const { return radius; }
    /**
     * @return Return rotation of Circle
     */
    inline double getRotation() const { return rot; }

    /**
     * @brief Sets cosine
     * @param val New value to be set
     */
    inline void setCosRad(double val) { this->cosRad = val; }
    /**
     * @brief Set sinus
     * @param val New value to be set
     */
    inline void setSinRad(double val) { this->sinRad = val; }
    /**
     * @brief Set rotation
     * @param rot New rotation of circle
     */
    inline void setRotation(double rot) { this->rot = rot; }
    /**
     * @brief Set radius
     * @param rad New radius to be set
     */
    inline void setRadius(double rad) { this->radius = rad; }

    /**
     * @brief Calculates sinus and cosine values and stores them into
     * cosRad and sinRad
     */
    void calculateSinCos();
    /**
     * @brief Calculates sinus and cosine values and stores them into
     * cosRad and sinRad
     * @param rot Rotation based on which sin/cos will be calculated
     */
    void calculateSinCos(double rot);
    /**
     * @brief Calculates sinus and cosine values and stores them into
     * cosRad and sinRad
     * @param cosRad Output variable to which cosine value will be set
     * @param sinRad Output variable to which sinus value will be set
     * @param rot Rotation based on which sin/cos will be calculated
     */
    void calculateSinCos(double* cosRad, double* sinRad, double* rot);

    /**
     * @brief Checks if two circles intersect
     * @param a Pointer to first Circle
     * @param b Pointer to second Circle
     * @return True if Circles intersect
     */
    static inline bool intersect(Circle* a, Circle* b)
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

    /**
     * @brief Checks if two circles intersect
     * @param a Reference to first Circle
     * @param b Reference to second Circle
     * @return True if Circles intersect
     */
    static inline bool intersect(Circle& a, Circle& b)
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
