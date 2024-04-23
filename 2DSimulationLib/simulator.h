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
#include "simulationcore.h"

class Simulator : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Constructor of Simulator object
     * @param robots Reference to vector of Robots
     * @param scene Reference to scene for updating it
     * @param maxThreads Maximum allowed threads to use
     * @param timer Pointer to QTimer that will call Simulation
     */
    Simulator(std::vector<std::unique_ptr<AutoRobot>> &robots, QGraphicsScene &scene, size_t maxThreads, QTimer* timer);

    ~Simulator();

    /**
     * @brief Initializes SimulationCores and starts it loop
     */
    void initializeCores();

    /**
     * @brief Starts simulation
     */
    void runSimulation();

    /**
     * @brief Stops simulation
     */
    void stopSimulation();

    /**
     * @brief Sets period of QTimer updating in milliseconds
     * @param milliSeconds Value to be set to the QTimer
     */
    void setTimerPeriod(int milliSeconds);

    /**
     * @return Returns last simulation cycle time
     */
    long long getCycleTime();

    /**
     * @brief Balances workload between SimulationCores
     */
    void balanceCores();

private:
    /**
     * @brief Reference to the scene for updating
     */
    QGraphicsScene& scene;

    /**
     * @brief Reference to the vector of Robots
     */
    std::vector<std::unique_ptr<AutoRobot>>& robots;

    /**
     * @brief Vector of SimulationCores
     */
    std::vector<std::unique_ptr<SimulationCore>> simCores;

    /**
     * @brief Vector of threads that run SimulationCores
     */
    std::vector<std::unique_ptr<std::thread>> simThreads;

    /**
     * @brief Pointer to global timer
     */
    QTimer* timer;

    /**
     * @brief Mutex for SimulationCore synchronization
     */
    std::mutex mutex;

    /**
     * @brief Condition for waking SimulationCores
     */
    std::condition_variable wakeCores;

    /**
     * @brief Bool value for keeping SimulationCores running
     */
    bool keepSimCoresRunning;

    /**
     * @brief Number of maximum threads
     */
    size_t maxThreads;

    /**
     * @brief Timer period in milliseconds
     */
    int timerPeriod_ms = 20;

    /**
     * @brief Last period of cycle timer
     */
    long long cycleTime = 0;

    /**
     * @brief Creates SimulationCore in another thread and runs in main loop
     * for simulation
     * @param simCores Reference to vector of SimulationCores
     * @param robots Reference to vector of AutoRobots
     * @param wakeCores Pointer to condition variable for waking SimulationCore
     * @param mutex Pointer to mutex for synchronizing SimulationCore
     * @param keepSimulating Pointer to bool value for turning off simulation loop
     */
    static void createSimulationCore(   std::vector<std::unique_ptr<SimulationCore>>& simCores,
                                     std::vector<std::unique_ptr<AutoRobot>>& robots,
                                     std::condition_variable* wakeCores,
                                     std::mutex* mutex,
                                     bool* keepSimulating);

protected slots:
    /**
     * @brief Function that calls all SimulationCores to simulate once
     */
    void simulationCycle();
};

#endif // SIMULATOR_H
