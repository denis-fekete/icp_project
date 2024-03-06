#include "simulator.h"
#include <thread>
#include <chrono>

Simulator::Simulator(std::vector<AutoRobot*>* robots, std::vector<Obstacle*>* obstacles, int numOfRobotsPerThread)
{
    this->robots = robots;
    this->obstacles = obstacles;
    this->numOfRobotsPerThread = numOfRobotsPerThread;

    this->timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(SimulationCycle()));
}

void Simulator::SimulationCycle()
{
    auto beggining = std::chrono::high_resolution_clock::now();

    std::vector<std::thread*> simThreads;

    const size_t vectorMaxSize = robots->size();

    // Split simulation calculating between threads
    for(size_t i = 0; i < vectorMaxSize; i += numOfRobotsPerThread)
    {

        simThreads.push_back(new std::thread(SimulateGroup,
                                             robots,
                                             i,
                                             (((i + numOfRobotsPerThread - 1) > vectorMaxSize)? vectorMaxSize : i + numOfRobotsPerThread - 1)
                                             ));
    }

    // Wait for all threads to end
    for(size_t i = 0; i < simThreads.size(); i++)
    {
        simThreads.at(i)->join();
    }

    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - beggining);
    this->cycleTime = duration.count();
}

void Simulator::SimulateGroup(std::vector<AutoRobot*>* robots, const size_t start, const size_t end)
{
    for(size_t i = 0; start + i < end; i++)
    {
        robots->at(start+i)->Simulate();
    }
}

void Simulator::RunSimulation()
{
    this->timer->start(timerPeriod_ms);
}

void Simulator::StopSimulation()
{
    this->timer->stop();
}

void Simulator::SetTimerPeriod(int milliSeconds)
{
    this->timerPeriod_ms = milliSeconds;
}

long long Simulator::GetCycleTime()
{
    return this->cycleTime;
}
