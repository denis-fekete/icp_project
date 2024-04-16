#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>

#include <QObject>
#include <QTimer>
#include <QGraphicsScene>

#include "../Visualization/autorobot.h"
#include "../Visualization/obstacle.h"
#include "simulationcore.h"

class Simulator : public QObject
{
    Q_OBJECT

public:
    Simulator(std::vector<AutoRobot*>* robots, QGraphicsScene* scene, size_t maxThreads);
    void initializeCores();
    void runSimulation();
    void stopSimulation();
    void setTimerPeriod(int milliSeconds);
    long long getCycleTime();

protected:
    QGraphicsScene* scene;
    std::vector<AutoRobot*>* robots;

    std::vector<SimulationCore*>* simCores;
    std::vector<std::thread*>* simThreads;

    // datatypes for synchornization
    std::mutex mutex;
    std::condition_variable wakeCores;
    bool keepSimulating;

    // performence parameters
    size_t maxThreads;
    QTimer* timer;
    int timerPeriod_ms = 20;
    long long cycleTime = 0;

private:
    static void createSimulationCore(   std::vector<SimulationCore*>* simCores,
                                        std::vector<AutoRobot*>* robots,
                                        std::condition_variable* wakeCores,
                                        std::mutex* mutex,
                                        bool* keepSimulating);

protected slots:
    void simulationCycle();
};

#endif // SIMULATOR_H
