#include "simulationcore.h"

SimulationCore::SimulationCore(std::vector<std::unique_ptr<AutoRobot>>& allRobots, std::condition_variable* wakeCondition, std::mutex* mutex, bool* simulate) : allRobots(allRobots)
{
    myRobotsStart = 0;
    myRobotsEnd = 0;
    this->simulate = simulate;

    this->wakeCondition = wakeCondition;
    this->mutex = mutex;
    lastDuration = 0;
}

#define LOG_PERFORMACE

void SimulationCore::runSimulation()
{
    auto lock = new std::unique_lock<std::mutex>(*mutex);
    while(*simulate)
    {
        // wait until i am notified
        wakeCondition->wait(*lock);
#ifdef LOG_PERFORMACE
        auto beggining = std::chrono::high_resolution_clock::now();
#endif

        // if start and end are same, reset loop
        if(myRobotsStart == myRobotsEnd)
            continue;

        for(size_t index = myRobotsStart; index < myRobotsEnd; index++)
        {
            allRobots.at(index)->simulate();
        }
#ifdef LOG_PERFORMACE
        auto end = std::chrono::high_resolution_clock::now();
        lastDuration = std::chrono::duration_cast<std::chrono::microseconds>(end - beggining).count();
#endif
    }

    // delete lock;
    // lock->release();
}
