#include "simulator.h"
#include <chrono>

Simulator::Simulator(std::vector<std::unique_ptr<AutoRobot>> &robots, QGraphicsScene &scene, size_t maxThreads) : scene(scene), robots(robots)
{
    this->maxThreads = maxThreads;

    connect(&timer, SIGNAL(timeout()), this, SLOT(simulationCycle()));
}


void Simulator::simulationCycle()
{
    if(robots.size() == 0 || simCores.size() == 0)
    {
        return;
    }

    auto beggining = std::chrono::high_resolution_clock::now();

    size_t workPerThread = robots.size() / maxThreads;
    size_t overtime = robots.size() % maxThreads;
    // set work for simulation cores
    size_t lastEnd = 0;
    size_t newEnd = 0;
    for(size_t index = 0; index < maxThreads; index++)
    {
        // set new as last end + work per one thread
        newEnd = lastEnd + workPerThread;

        // if overtime is not 0, add robot to thread
        if(overtime > 0)
        {
            // add to the end
            newEnd++;
            overtime--;
        }

        // set workforce for the cores
        simCores.at(index)->setIndexes(lastEnd, newEnd);
        lastEnd = newEnd;
    }

    wakeCores.notify_all();

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - beggining);
    this->cycleTime = duration.count();

    scene.update();
}


void Simulator::initializeCores()
{
    for(size_t i = 0; i < maxThreads; i++)
    {
        simThreads.push_back(std::make_unique<std::thread> (&Simulator::createSimulationCore, std::ref(simCores), std::ref(robots), &wakeCores, &mutex, &keepSimulating));
    }
}

void Simulator::createSimulationCore(std::vector<std::unique_ptr<SimulationCore>>& simCores,
                                     std::vector<std::unique_ptr<AutoRobot>>& robots,
                                     std::condition_variable* wakeCores,
                                     std::mutex* mutex,
                                     bool* keepSimulating)
{
    simCores.push_back(std::make_unique<SimulationCore> (robots, wakeCores, mutex, keepSimulating));
    simCores.back().get()->runSimulation();
}

void Simulator::runSimulation()
{
    this->timer.start(timerPeriod_ms);
}

void Simulator::stopSimulation()
{
    this->timer.stop();
}

void Simulator::setTimerPeriod(int milliSeconds)
{
    this->timerPeriod_ms = milliSeconds;
}

long long Simulator::getCycleTime()
{
    return this->cycleTime;
}
