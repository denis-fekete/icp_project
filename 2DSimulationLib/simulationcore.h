#ifndef SIMULATIONCORE_H
#define SIMULATIONCORE_H

#include <vector>
#include <condition_variable>
#include <mutex>

#include "../QtSpecific/autorobot.h"

class SimulationCore
{
private:
    /**
     * @brief allRobots Pointer to the vector of all robots
     */
    std::vector<std::unique_ptr<AutoRobot>>* allRobots;

    /**
     * @brief myRobotsStart Index from which this core will call simulation on robots
     */
    size_t myRobotsStart;

    /**
     * @brief myRobotsEnd Index to which this core will call simulation on robots
     */
    size_t myRobotsEnd;

    /**
     * @brief wakeCondition Condition for waking this simulation core
     */
    std::condition_variable* wakeCondition;

    /**
     * @brief mutex Mutex for lock creation
     */
    std::mutex* mutex;

    /**
     * @brief simulate Whenever simulation core should keep going
     */
    bool* simulate;
public:

    /**
     * @brief Constructor of SimulationCore object
     * @param allRobots Reference to vector of all robots
     * @param wakeCondition Condition for waking this SimulationCore  when suspended
     * @param mutex Mutex for suspending this SimulationCore
     * @param simulate Bool value for checking if SimulationCore should
     * keep simulating
     */
    SimulationCore(std::vector<std::unique_ptr<AutoRobot>>* allRobots, std::condition_variable* wakeCondition, std::mutex* mutex, bool* simulate);

    /**
     * @brief Loops and simulates robots it is pointing to until `simulate`
     * is set to false
     */
    void runSimulation();

    /**
     * @brief Sets stard and end indexes that this SimulationCore will simulate
     * @param start Start index
     * @param end End index
     */
    inline void setIndexes(size_t start, size_t end )
    {
        myRobotsStart = start;
        myRobotsEnd = end;
    }

    /**
     * @brief lastDuration Last duration of the simulation cycle
     */
    double lastDuration;
};

#endif // SIMULATIONCORE_H
