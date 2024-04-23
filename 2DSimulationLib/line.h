#ifndef LINE_H
#define LINE_H

#include "point.h"
#include "QDebug"

class Line
{
public:
    /**
     * @brief start Point at which line starts
     */
    Point start;
    /**
     * @brief end Point at which line ends
     */
    Point end;

    /**
     * @brief Line Constructor of Line object
     * @param start Point in which Line will start
     * @param end Point in which Line will end
     */
    Line(Point start, Point end);
    /**
     * @brief Line Constructor of Line object
     * @param startX X position in which Line will start
     * @param startY Y position in which Line will start
     * @param endX X position in which Line will end
     * @param endY Y position in which Line will end
     */
    Line(double startX, double startY, double endX, double endY);

    /**
     * @brief Check if this Line intersects with `other` Line, if true sets
     * Point of intersection into `intersectionPoint`
     * @param other Reference to other line
     * @param intersectionPoint Point of intersection to which position will be set
     * @return True if lines intersect
     */
    bool intersectsWith(Line& other, Point& intersectionPoint);

    /**
     * @brief Check if Line `a` intersects with `b` Line, if true sets
     * Point of intersection into `intersectionPoint`
     * @param a Reference to first Line
     * @param b Reference to second Line
     * @param intersectionPoint Point of intersection to which position will be set
     * @return True if lines intersect
     */
    static bool linesIntersects(Line& a, Line& b, Point& intersectionPoint);

    /**
     * @brief Checks if Point is on left side of `this` Line
     * @param p Reference to the Point
     * @return True if Point is on left side of this Line
     */
    bool pointOnLeftSide(Point& p);

    /**
     * @brief Checks if Point is on left side of the two Points that would create a Line
     * @param p Point which positon is checked
     * @param lineStart Starting point of the "Line"
     * @param lineEnd Ending point of the "Line"
     * @return True if Point `p` is on left side
     */
    static bool pointOnLeftSide(Point& p, Point& lineStart, Point& lineEnd);

    /**
     * @brief Checks if Point is on left side of `line` Line
     * @param p Reference to the Point
     * @return True if Point is on left side of this Line
     */
    static bool pointOnLeftSide(Point& p, Line& line);
};

#endif // LINE_H
