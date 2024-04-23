#include "simulator.h"
#include <chrono>
#include <iostream>
#define LOG_PERFORMACE

Simulator::Simulator(std::vector<std::unique_ptr<AutoRobot>> &robots, QGraphicsScene &scene, size_t maxThreads, QTimer* timer) : scene(scene), robots(robots)
{
    this->maxThreads = maxThreads;
    this->timer = timer;
    keepSimCoresRunning = false;
    connect(timer, SIGNAL(timeout()), this, SLOT(simulationCycle()));
}

void Simulator::simulationCycle()
{
    if(robots.size() == 0)
    {
        return;
    }

    if(maxThreads == 0)
    {
#ifdef LOG_PERFORMACE
        auto beggining = std::chrono::high_resolution_clock::now();
#endif
        for(size_t index = 0; index < robots.size(); index++)
        {
            robots.at(index)->simulate();
        }
#ifdef LOG_PERFORMACE
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - beggining);
        cycleTime = duration.count();
#endif
    }
    else
    {
        wakeCores.notify_all();
#ifdef LOG_PERFORMACE
        cycleTime = 0;
        for(size_t index = 0; index < maxThreads; index++)
        {
            cycleTime += simCores.at(index).get()->lastDuration;
        }
        cycleTime /= 4;
#endif
    }


    scene.update();
}


void Simulator::initializeCores()
{
    keepSimCoresRunning = true;
    if(maxThreads > 1)
    {
        for(size_t i = 0; i < maxThreads; i++)
        {
            simThreads.push_back(std::make_unique<std::thread> (&Simulator::createSimulationCore, std::ref(simCores), std::ref(robots), &wakeCores, &mutex, &keepSimCoresRunning));
            std::this_thread::sleep_for(std::chrono::duration<double>(1));
        }
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
    this->timer->start(timerPeriod_ms);
}

void Simulator::stopSimulation()
{
    this->timer->stop();
}

void Simulator::setTimerPeriod(int milliSeconds)
{
    this->timerPeriod_ms = milliSeconds;
}

long long Simulator::getCycleTime()
{
    return this->cycleTime;
}

Simulator::~Simulator()
{
    this->keepSimCoresRunning = false;
    wakeCores.notify_all();

    this->timer->stop();
}

void Simulator::balanceCores()
{
    if(maxThreads == 0)
    {
        return;
    }
    if(simCores.size() != maxThreads)
    {
        exit(-1);
    }

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
        std::cout << "core=" << index << " , start=" << lastEnd << ", end=" << newEnd << std::endl;
        lastEnd = newEnd;
    }
}
