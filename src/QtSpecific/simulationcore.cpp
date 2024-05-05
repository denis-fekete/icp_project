/**
 * @file simulationcore.cpp
 * @brief Implementation of SimulationCore class methods
 *
 * @author Denis Fekete (xfeket01@fit.vutbr.cz)
 */

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

#define LOG_TIME

#ifdef LOG_TIME
#include <thread>
#include <iostream>
#endif

void SimulationCore::runSimulation()
{
    auto lock = std::make_unique<std::unique_lock<std::mutex>> (*mutex);

    #ifdef LOG_TIME
        auto beforeWait = std::chrono::high_resolution_clock::now();
        auto startCalc = beforeWait;
        auto endCalc = beforeWait;
        #define castToMilis(time) std::chrono::duration<double, std::milli>(time).count()
    #endif

    while(*simulate)
    {
        #ifdef LOG_TIME
            beforeWait = std::chrono::high_resolution_clock::now();
        #endif
        // wait until i am notified
        wakeCondition->wait(*lock);

        #ifdef LOG_TIME
            startCalc = std::chrono::high_resolution_clock::now();
        #endif

        // if start and end are same, reset loop
        if(myRobotsStart == myRobotsEnd)
            continue;

        for(size_t index = myRobotsStart; index < myRobotsEnd; index++)
        {
            allRobots->at(index)->simulate();
        }

        #ifdef LOG_TIME
            endCalc = std::chrono::high_resolution_clock::now();
            std::cout << "Thread id: " << std::this_thread::get_id() << ", robot range(" <<
                myRobotsStart << ", " << myRobotsEnd << "), " <<"waited for simulator: " <<
                castToMilis(startCalc - beforeWait) << "ms, calculated: " <<
                castToMilis(endCalc - startCalc) << "ms\n" << std::flush;
        #endif
    }

    // delete lock;
    lock->release();
}
