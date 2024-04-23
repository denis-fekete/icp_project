#include "simulator.h"

#define LOG_PERFORMACE
#ifdef LOG_PERFORMACE
    #include <chrono>
    #include <iostream>
#endif

Simulator::Simulator(std::vector<std::unique_ptr<AutoRobot>> &robots, QGraphicsScene &scene, size_t maxThreads, QTimer* timer) : scene(scene), robots(robots)
{
    this->maxThreads = maxThreads;
    this->timer = timer;
    keepSimulating = false;

    connect(timer, SIGNAL(timeout()), this, SLOT(simulationCycle()));
}

void Simulator::simulationCycle()
{
    if(robots.size() == 0)
        return;
    if(!keepSimulating)
        return;

    if(maxThreads <= 1)
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
        std::cout << duration.count() << "ms" << std::endl;
#endif
    }
    else
    {
        wakeCores.notify_all();
#ifdef LOG_PERFORMACE
        double avrg = 0;
        for(size_t index = 0; index < maxThreads; index++)
        {
            avrg += simCores.at(index).get()->lastDuration;
        }
        std::cout << "avrg: " <<avrg / simCores.size() << "ms\n";
#endif
    }

    scene.update();
}

void Simulator::balanceCores()
{
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
        if(simCores.size() != maxThreads)
        {
            std::cout << simCores.size() << std::endl;
            exit(-2);
        }
        simCores.at(index)->setIndexes(lastEnd, newEnd);
        lastEnd = newEnd;
    }
}

void Simulator::initializeCores()
{
    if(maxThreads > 1)
    {
        for(size_t i = 0; i < maxThreads; i++)
        {
            std::cout << "created sim thread num" << i << std::endl;
            simThreads.push_back(std::make_unique<std::thread> (&Simulator::createSimulationCore, std::ref(simCores), std::ref(robots), &wakeCores, &mutex, &keepSimulating, &simCoreInit));
            std::this_thread::sleep_for(std::chrono::duration<int>(1));
        }
    }
}

void Simulator::createSimulationCore(std::vector<std::unique_ptr<SimulationCore>>& simCores,
                                     std::vector<std::unique_ptr<AutoRobot>>& robots,
                                     std::condition_variable* wakeCores,
                                     std::mutex* mutex,
                                     bool* keepSimulating,
                                     std::mutex* simCoreInit)

{
    std::unique_lock<std::mutex> lock(*simCoreInit);
    qDebug("created sim core");
    simCores.push_back(std::make_unique<SimulationCore> (robots, wakeCores, mutex, keepSimulating));
    lock.unlock();

    simCores.back().get()->runSimulation();
}

void Simulator::runSimulation()
{
    keepSimulating = true;
    this->timer->start(timerPeriod_ms);
}

void Simulator::stopSimulation()
{
    keepSimulating = false;
    this->timer->stop();
}

void Simulator::setTimerPeriod(int milliSeconds)
{
    timerPeriod_ms = milliSeconds;
}

long long Simulator::getCycleTime()
{
    return this->cycleTime;
}

Simulator::~Simulator()
{
    this->keepSimulating = false;
    wakeCores.notify_all();

    this->timer->stop();
}
