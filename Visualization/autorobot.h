#ifndef AUTOROBOT_H
#define AUTOROBOT_H

#include <QObject>
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QTimer>
#include <QTime>
#include <QDebug>

#include <vector>

#include "obstacle.h"
#include "../Simulation/robot.h"

class AutoRobot : public QGraphicsItem
{
    // Q_OBJECT
protected:
    Robot* sim; // Simulation of physics for robot
    // QGraphicsEllipseItem* graphics;
    // QGraphicsRectItem* collider;
    std::vector<Obstacle*>* obstacles;

    // Robot parameters
    QColor color;
    double speed;
    double turnAngle;
    short signed int turnDirection;

    // Internal
    QTimer* timer;

public:
    AutoRobot(double x, double y, double radius, double rot,
              double detRadius, QColor color, double speed,
              double turnAngle, bool turnRight,
              std::vector<Obstacle*>* obstaclesPointer);

    ~AutoRobot();
    void initialize(QGraphicsScene& scene);

    Robot* getSimulationInfo();
    QGraphicsEllipseItem* getGraphics();
    QGraphicsRectItem* getCollider();

    void setUnselected();
    void setSelected();

    inline void moveRobot(double distance) { sim->moveForward(distance); }
    inline void rotateRobot(double angle) { sim->rotate(angle); }
    void simulate();

    Point lastMoveDelta;

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

    inline QPoint point2QPoint(Point point) { return QPoint(point.getX(), point.getY()); };
    bool initialized;

    static void addRobotToWorld(double x, double y, double radius, double rot,
                                double detRadius, QColor color, double speed,
                                double turnAngle, bool turnRight,
                                std::vector<Obstacle*>* obstaclesPointer,
                                std::vector<AutoRobot*>& robots, QGraphicsScene& scene);
};

#endif // AUTOROBOT_H
