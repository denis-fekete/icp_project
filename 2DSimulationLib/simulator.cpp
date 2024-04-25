#include "simulator.h"
#include <chrono>
// #define LOG_PERFORMACE


Simulator::Simulator(QGraphicsScene &scene, size_t maxThreads, QTimer* timer) : scene(scene)
{
    this->maxThreads = maxThreads;
    this->timer = timer;
    activeRobot = nullptr;
    activeObstacle = nullptr;
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


    for(size_t index = 0; index < robots.size(); index++)
    {
         robots.at(index)->positionUpdate();
    }
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
            simThreads.push_back(std::make_unique<std::thread> (&Simulator::createSimulationCore, &simCores, &robots, &wakeCores, &mutex, &keepSimCoresRunning));
        }
    }
}

void Simulator::createSimulationCore(std::vector<std::unique_ptr<SimulationCore>>* simCores,
                                     std::vector<std::unique_ptr<AutoRobot>>* robots,
                                     std::condition_variable* wakeCores,
                                     std::mutex* mutex,
                                     bool* keepSimulating)
{
    simCores->push_back(std::make_unique<SimulationCore> (robots, wakeCores, mutex, keepSimulating));
    simCores->back().get()->runSimulation();
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
        lastEnd = newEnd;
    }
}


void Simulator::addAutomaticRobot(double x, double y, double radius, double rot,
                                     double detRadius, QColor color, double speed,
                                     double turnAngle, bool turnRight)
{
    robots.push_back(std::make_unique<AutoRobot> (x, y, radius, rot, detRadius, color, speed, turnAngle, turnRight, &colliders, &activeRobot));
    colliders.push_back(robots.back().get()->getSim().getColliderInner());

    robots.back()->initialize();

    scene.addItem(robots.back().get());

    balanceCores();
    scene.update();
}

void Simulator::addObstacle(double x, double y, double w, double h, double rot,
                                  QColor& color)
{
    obstacles.push_back(std::make_unique<Obstacle>(x, y, w, h, rot, color, &activeObstacle));
    colliders.push_back(obstacles.back().get()->getSimulationRectangle());

    obstacles.back()->initialize();

    scene.addItem(obstacles.back().get());

    scene.update();
}

void Simulator::setActiveRobot(size_t id)
{
    if(activeRobot != nullptr)
    {
        activeRobot->setUnselected();
    }
    activeRobot = robots.at(id).get();
    activeRobot->setSelected();
}

void Simulator::setActiveObstacle(size_t id)
{
    if(activeObstacle != nullptr)
    {
        activeObstacle->setUnselected();
    }
    activeObstacle = obstacles.at(id).get();
    activeObstacle->setSelected();
}

void Simulator::deleteRobot(size_t id)
{
    Rectangle* colliderToDelete = robots.at(id).get()->getSim().getColliderInner();
    for(size_t i = id; i < colliders.size(); i++)
    {
        if(colliders.at(i) == colliderToDelete)
        {
            colliders.erase(colliders.begin() + i);
            break;
        }
    }

    robots.erase(robots.begin() + id);
    activeRobot = nullptr;
}

void Simulator::deleteObstacle(size_t id)
{
    Rectangle* colliderToDelete = obstacles.at(id).get()->getSim();
    for(size_t i = id; i < colliders.size(); i++)
    {
        if(colliders.at(i) == colliderToDelete)
        {
            colliders.erase(colliders.begin() + i);
            break;
        }
    }

    obstacles.erase(obstacles.begin() + id);
    activeObstacle = nullptr;
}
