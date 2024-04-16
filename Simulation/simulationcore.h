#ifndef SIMULATIONCORE_H
#define SIMULATIONCORE_H

#include <vector>
#include <condition_variable>
#include <mutex>

#include "../Visualization/autorobot.h"

class SimulationCore
{
private:
    // pointer to the vector of all robots
    std::vector<AutoRobot*>* allRobots;
    // index from which this core will call simulation on robots
    size_t myRobotsStart;
    // index to which this core will call simulation on robots
    size_t myRobotsEnd;
    // condition for waking this simulation core
    std::condition_variable* wakeCondition;
    // lock for waiting
    std::unique_lock<std::mutex>* lock;
    // whenever simulation core should end
    bool* simulate;
public:
    SimulationCore(std::vector<AutoRobot*>* allRobots, std::condition_variable* wakeCondition, std::mutex* mutex, bool* simulate);

    void runSimulation();

    inline void setIndexes(size_t start, size_t end )
    {
        myRobotsStart = start;
        myRobotsEnd = end;
    }
};

#endif // SIMULATIONCORE_H
