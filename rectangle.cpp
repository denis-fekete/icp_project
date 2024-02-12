#include <rectangle.h>

Rectangle::Rectangle() {}

Rectangle::Rectangle(double x, double y, double w, double h, double rot) : Point(x, y, rot)
{
    this->w = w;
    this->h = h;

    radius = sqrt( (w * w +  h * h) / 4);

    UpdatePoints();
}

Rectangle::~Rectangle() {}

void Rectangle::MoveTo(Point p)
{
    // Update center point
    x = p.x;
    y = p.y;

    // Call UpdatePoints to update points based on rotation
    this->UpdatePoints();
}


void Rectangle::MoveForward(double distance)
{
    // Calcualte delta value for moving in X and Y direction
    double xDelta = GetCosRad() * distance;
    double yDelta = GetSinRad() * distance;

    // Apply deltas to the current possition
    x += xDelta;
    y += yDelta;

    const Point p(xDelta, yDelta);

    LB = LB + p;
    RB = RB + p;
    RT = RT + p;
    LT = LT + p;
}

void Rectangle::Rotate(double angle)
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

    // Update points / corners
    UpdatePoints();
}

void UpdateSinglePoint(Point* center , Point* p, double cosRad, double sinRad)
{
    // Calculate translated position
    double tmpX = p->x - center->x;
    double tmpY = p->y - center->y;
    // Calculate rotated position
    double rotatedX = (tmpX * cosRad) - (tmpY * sinRad);
    double rotatedY = (tmpX * sinRad) + (tmpY * cosRad);
    // Transle back to original position with rotated position
    p->x = rotatedX + center->x;
    p->y = rotatedY + center->y;
}

void CalcualteCornersWithNoRotation(Rectangle* rect)
{
    // Calculate constants width/2 and heigth/2
    const auto w2 = rect->w / 2;
    const auto h2 = rect->h / 2;

    // Calculate corner points with no rotation
    rect->LB = Point(rect->x - w2, rect->y - h2);
    rect->LT = Point(rect->x - w2, rect->y + h2);
    rect->RB = Point(rect->x + w2, rect->y - h2);
    rect->RT = Point(rect->x + w2, rect->y + h2);
}

void Rectangle::UpdatePoints(double cosRad, double sinRad)
{
    CalcualteCornersWithNoRotation(this);
    Point center(this->x, this->y);

    // Update stored sinus and cosinus values
    this->SetCosRad(cosRad);
    this->SetSinRad(sinRad);

    // Calculate corners with rotation
    UpdateSinglePoint(&center, &LB, cosRad, sinRad);
    UpdateSinglePoint(&center, &LT, cosRad, sinRad);
    UpdateSinglePoint(&center, &RB, cosRad, sinRad);
    UpdateSinglePoint(&center, &RT, cosRad, sinRad);
}

void Rectangle::UpdatePoints()
{
    CalcualteCornersWithNoRotation(this);

    Point center(this->x, this->y);

    // Update stored sinus and cosinus values
    this->CalculateSinCos(this->rot);

    const double cosRad = this->GetCosRad();
    const double sinRad = this->GetSinRad();

    // Calculate corners with rotation
    UpdateSinglePoint(&center, &LB, cosRad, sinRad);
    UpdateSinglePoint(&center, &LT, cosRad, sinRad);
    UpdateSinglePoint(&center, &RB, cosRad, sinRad);
    UpdateSinglePoint(&center, &RT, cosRad, sinRad);
}

bool LineIntersects(Point* startA, Point* endA, Point* startB, Point* endB, Point *intersectionPoint)
{
    double denominator = (  (endB->y - startB->y) * (endA->x - startA->x)
                          - (endB->x - startB->x) * (endA->y - startA->y)  );

    // Lines are parallel
    if(denominator == 0)
    {
        return false;
    }

    double ua = (   (endB->x - startB->x) * (startA->y - startB->y)
                 -   (endB->y - startB->y) * (startA->x - startB->x))
                / denominator;
    double ub = (   (endA->x - startA->x) * (startA->y - startB->y)
                 -   (endA->y - startA->y) * (startA->x - startB->x))
                / denominator;

    // is the intersection along the segments
    if(ua < 0 || ua > 1 || ub < 0 || ub > 1)
    {
        return false;
    }

    intersectionPoint->x = startA->x + ua * (endA->x - startA->x);
    intersectionPoint->y = startA->y + ua * (endA->y - startA->y);

    return true;
}

void BreakIntoEdges(Point* pStart, Point* pEnd, Rectangle* rect, unsigned edge)
{
    switch (edge)
    {
    case 1:
        *pStart = rect->LB;
        *pEnd = rect->RB;
        break;
    case 2:
        *pStart = rect->RB;
        *pEnd = rect->RT;
        break;
    case 3:
        *pStart = rect->RT;
        *pEnd = rect->LT;
        break;
    case 4:
        *pStart = rect->LT;
        *pEnd = rect->LB;
        break;
    default:
        break;
    }

    // switch (edge)
    // {
    // case 1:
    //     *pStart = &(rect->LB);
    //     *pEnd = &(rect->RB);
    //     break;
    // case 2:
    //     *pStart = &(rect->RB);
    //     *pEnd = &(rect->RT);
    //     break;
    // case 3:
    //     *pStart = &(rect->RT);
    //     *pEnd = &(rect->LT);
    //     break;
    // case 4:
    //     *pStart = &(rect->LT);
    //     *pEnd = &(rect->LB);
    //     break;
    // default:
    //     break;
    // }
}

bool OnLeftSideOfLine(Point S, Point A, Point B)
{
    auto res = (B.x - A.x) * (S.y - A.y) - (S.x - A.x) * (B.y - A.y);

    // If res >= 0 point is on the line, or on the left side
    return (res >= 0) ? true : false;
}

bool PointInRectangle(Point* point, Rectangle* rect)
{
    if(OnLeftSideOfLine(*point, rect->LB, rect->RB))
    {
        // std::cout << "LB-RB\n";
        if(OnLeftSideOfLine(*point, rect->RB, rect->RT))
        {
            // std::cout << "RB-RT\n";
            if(OnLeftSideOfLine(*point, rect->RT, rect->LT))
            {
                // std::cout << "RT-LT\n";
                if(OnLeftSideOfLine(*point, rect->LT, rect->LB))
                {
                    // std::cout << "LT-LB\n";
                    return true;
                }
            }
        }
    }

    return false;
}

#define POLYGON_EDGE_COUNT 4
bool Rectangle::Intersects(Rectangle* other)
{
    // Check edges of rectangle
    if(PointInRectangle(&(this->LB), other)) {return true; }
    if(PointInRectangle(&(this->RB), other)) {return true; }
    if(PointInRectangle(&(this->RT), other)) {return true; }
    if(PointInRectangle(&(this->LT), other)) {return true; }

    // Check whenever edges of rectangle intesect
    for(int i = 0; i < POLYGON_EDGE_COUNT; i++)
    {
        Point startA;
        Point endA;
        BreakIntoEdges(&startA, &endA, this, i);

        for (int j = 0; j < POLYGON_EDGE_COUNT; j++)
        {
            Point startB;
            Point endB;
            BreakIntoEdges(&startB, &endB, other, j);

            Point intersectionPoint;
            // Check if lines intersect
            if(LineIntersects(&startA, &endA, &startB, &endB, &intersectionPoint))
            {
                // std::cout << "IntPoint : (" << intersectionPoint.x << ", " << intersectionPoint.y << ")\n";
                // Check if found point is in both rectangles
                if(PointInRectangle(&intersectionPoint, this))
                {
                    if(PointInRectangle(&intersectionPoint, other))
                    {
                        return true;
                    }
                }
            }
        }
    }

    return false;
}
#undef POLYGON_EDGE_COUNT
