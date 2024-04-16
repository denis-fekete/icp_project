#include "simulator.h"
#include <chrono>

Simulator::Simulator(std::vector<AutoRobot*>* robots, QGraphicsScene* scene, size_t maxThreads)
{
    this->robots = robots;
    this->scene = scene;
    this->maxThreads = maxThreads;

    this->timer = new QTimer();
    this->simCores = new std::vector<SimulationCore*>;
    this->simThreads = new std::vector<std::thread*>;

    connect(timer, SIGNAL(timeout()), this, SLOT(simulationCycle()));
}


void Simulator::initializeCores()
{

    for(size_t i = 0; i < maxThreads; i++)
    {
        simThreads->push_back( new std::thread(createSimulationCore, simCores, robots, &wakeCores, &mutex, &keepSimulating) );
    }
}


void Simulator::simulationCycle()
{
    if(robots->size() == 0 || simCores->size() == 0)
    {
        return;
    }

    auto beggining = std::chrono::high_resolution_clock::now();

    size_t workPerThread = robots->size() / maxThreads;
    size_t overtime = robots->size() % maxThreads;
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
        simCores->at(index)->setIndexes(lastEnd, newEnd);
        lastEnd = newEnd;
    }

    wakeCores.notify_all();

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - beggining);
    this->cycleTime = duration.count();

    scene->update();
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

void Simulator::createSimulationCore(   std::vector<SimulationCore*>* simCores,
                                 std::vector<AutoRobot*>* robots,
                                 std::condition_variable* wakeCores,
                                 std::mutex* mutex,
                                 bool* keepSimulating)
{
    SimulationCore* newCore = new SimulationCore(robots, wakeCores, mutex, keepSimulating);
    simCores->push_back(newCore);
    newCore->runSimulation();
}
