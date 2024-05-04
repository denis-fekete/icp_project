#include "simulationcore.h"

SimulationCore::SimulationCore(std::vector<BaseRobot *> *allRobots, std::condition_variable* wakeCondition, std::mutex* mutex, bool* simulate) : allRobots(allRobots)
{
    myRobotsStart = 0;
    myRobotsEnd = 0;
    this->simulate = simulate;

    this->wakeCondition = wakeCondition;
    this->mutex = mutex;
    lastDuration = 0;
}

void SimulationCore::runSimulation()
{
    auto lock = std::make_unique<std::unique_lock<std::mutex>> (*mutex);
    while(*simulate)
    {
        // wait until i am notified
        wakeCondition->wait(*lock);

        // if start and end are same, reset loop
        if(myRobotsStart == myRobotsEnd)
            continue;

        for(size_t index = myRobotsStart; index < myRobotsEnd; index++)
        {
            allRobots->at(index)->simulate();
        }

    }

    // delete lock;
    lock->release();
}
