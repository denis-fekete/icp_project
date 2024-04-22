#ifndef AUTOROBOT_H
#define AUTOROBOT_H

#include <QObject>
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QGraphicsScene>

#include <QTime>
#include <QDebug>

#include <vector>

#include "obstacle.h"
#include "../2DSimulationLib/robot.h"

class AutoRobot : public QGraphicsItem
{
    // Q_OBJECT
private:
    Robot sim; // Simulation of physics for robot
    std::vector<std::unique_ptr<Obstacle>>& obstacles;

    // Robot parameters
    QColor color;
    double speed;
    double turnAngle;
    short signed int turnDirection;

public:
    AutoRobot(double x, double y, double radius, double rot,
              double detRadius, QColor color, double speed,
              double turnAngle, bool turnRight,
              std::vector<std::unique_ptr<Obstacle>>& obstaclesPointer);

    void initialize(QGraphicsScene& scene);

    void setUnselected();
    void setSelected();

    inline void moveRobot(double distance) { sim.moveForward(distance); }
    inline void rotateRobot(double angle) { sim.rotate(angle); }
    void simulate();

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

    inline QPoint point2QPoint(Point point) { return QPoint(point.getX(), point.getY()); };
    bool initialized;

    static void addRobotToWorld(double x, double y, double radius, double rot,
                                double detRadius, QColor color, double speed,
                                double turnAngle, bool turnRight,
                                std::vector<std::unique_ptr<Obstacle>>& obstaclesPointer,
                                std::vector<std::unique_ptr<AutoRobot>>& robots, QGraphicsScene& scene);

    // getters
    inline Robot& getSim() { return this->sim; }
    inline QColor& getColor() { return this->color; }
    inline double getSpeed() { return this->speed; }
    inline double getTurnAngle() { return this->turnAngle; }
    inline short signed int getTurnDirection() { return this->turnDirection; }
};

#endif // AUTOROBOT_H
