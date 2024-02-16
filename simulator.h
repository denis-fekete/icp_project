#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <thread>
#include <vector>

#include <QObject>
#include <QTimer>

#include "autorobot.h"
#include "obstacle.h"

class Simulator : public QObject
{
    Q_OBJECT
public:
    Simulator(std::vector<AutoRobot*>* robots, std::vector<Obstacle*>* obstacles, int numOfRobotsPerThread);
    void RunSimulation();
    void StopSimulation();
    void SetTimerPeriod(int milliSeconds);

protected:
    std::vector<Obstacle*>* obstacles;
    std::vector<AutoRobot*>* robots;

    static int numOfRobotsPerThread;
    QTimer* timer;
    int timerPeriod_ms = 20;

    bool simulationEnabled;

    void SimulationCycle();
    void SimulateRobots();
};

#endif // SIMULATOR_H
