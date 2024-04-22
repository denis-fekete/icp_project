#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <memory>

#include <QObject>
#include <QTimer>
#include <QGraphicsScene>

#include "../QtSpecific/autorobot.h"
#include "../QtSpecific/obstacle.h"
#include "simulationcore.h"

class Simulator : public QObject
{
    Q_OBJECT

public:
    Simulator(std::vector<AutoRobot*>* robots, QGraphicsScene* scene, size_t maxThreads);
    Simulator(std::vector<std::unique_ptr<AutoRobot>> &robots, QGraphicsScene &scene, size_t maxThreads);
    ~Simulator();
    void initializeCores();
    void runSimulation();
    void stopSimulation();
    void setTimerPeriod(int milliSeconds);
    long long getCycleTime();

private:
    QGraphicsScene& scene;
    std::vector<std::unique_ptr<AutoRobot>>& robots;
    std::vector<std::unique_ptr<SimulationCore>> simCores;
    std::vector<std::unique_ptr<std::thread>> simThreads;

    QTimer timer;

    // datatypes for synchornization
    std::mutex mutex;
    std::condition_variable wakeCores;
    bool keepSimulating;

    // performence parameters
    size_t maxThreads;
    int timerPeriod_ms = 20;
    long long cycleTime = 0;

    static void createSimulationCore(   std::vector<std::unique_ptr<SimulationCore>>& simCores,
                                        std::vector<std::unique_ptr<AutoRobot>>& robots,
                                        std::condition_variable* wakeCores,
                                        std::mutex* mutex,
                                        bool* keepSimulating);

protected slots:
    void simulationCycle();
};

#endif // SIMULATOR_H
