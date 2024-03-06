#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <vector>

#include <QObject>
#include <QTimer>

#include "../Visualization/autorobot.h"
#include "../Visualization/obstacle.h"

class Simulator : public QObject
{
    Q_OBJECT

public:
    Simulator(std::vector<AutoRobot*>* robots, std::vector<Obstacle*>* obstacles, int numOfRobotsPerThread);
    Simulator();

    void RunSimulation();
    void StopSimulation();
    void SetTimerPeriod(int milliSeconds);
    long long GetCycleTime();
protected:
    std::vector<Obstacle*>* obstacles;
    std::vector<AutoRobot*>* robots;

    int numOfRobotsPerThread;
    QTimer* timer;
    int timerPeriod_ms = 20;
    long long cycleTime = 0;

    static void SimulateGroup(std::vector<AutoRobot*>* robots, const size_t start, const size_t end);

protected slots:
    void SimulationCycle();
};

#endif // SIMULATOR_H
