#include "simulator.h"

#include <algorithm>

// #define LOG_PERFORMACE

#ifdef LOG_PERFORMACE
    #include <chrono>
#endif

Simulator::Simulator(QGraphicsScene &scene, size_t maxThreads) : scene(scene)
{
    this->maxThreads = maxThreads;
    timerPeriod = 30; // default 30 ms period
    activeRobot = nullptr;
    activeObstacle = nullptr;
    keepSimCoresRunning = false;

    connect(&timerSim, SIGNAL(timeout()), this, SLOT(simulationCycle()));
    connect(&timerGraphics, SIGNAL(timeout()), this, SLOT(updateGraphicsPos()));
}


void Simulator::updateGraphicsPos()
{
    if(robots.size() == 0)
    {
        return;
    }

    for(size_t index = 0; index < robots.size(); index++)
    {
        robots.at(index)->positionUpdate();
    }
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
            robots.at(index)->positionUpdate();
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
            simThreads.push_back(std::make_unique<std::thread> (&Simulator::createSimulationCore, &simCores, &robots, &wakeCores, &mutex, &keepSimCoresRunning));
        }
    }
}

void Simulator::createSimulationCore(std::vector<std::unique_ptr<SimulationCore>>* simCores,
                                     std::vector<BaseRobot*>* robots,
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

    this->timerSim.stop();
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
    // cosnt for normalizing values
    const double timeNorm = 1000 / timerPeriod;

    // create new AutoRobot and add it to vector of autorobots
    autoRobots.push_back(std::make_unique<AutoRobot> (x, y, radius, rot,
                                                     detRadius, color,
                                                     speed / timeNorm,
                                                     turnAngle / timeNorm,
                                                     turnRight, &colliders, this));
    // add its collider to the vector of colliders
    colliders.push_back(autoRobots.back().get()->getSim()->getColliderInner());
    // add it do vector of all robots
    robots.push_back(autoRobots.back().get());

    // initialize it and add it the scene
    autoRobots.back()->initialize();
    scene.addItem(autoRobots.back().get());

    autoRobots.back().get()->positionUpdate();
    balanceCores();
    scene.update();
}

void Simulator::addManualRobot(double x, double y, double radius, double rot,
                                double detRadius, QColor color)
{
    // create new AutoRobot and add it to vector of manual robots
    manualRobots.push_back(std::make_unique<ManualRobot> (x, y, radius, rot, detRadius, color, &colliders, this));
    // add its collider to the vector of colliders
    colliders.push_back(manualRobots.back().get()->getSim()->getColliderInner());
    // add it do vector of all robots
    robots.push_back(manualRobots.back().get());

    // initialize it and add it the scene
    manualRobots.back()->initialize();
    scene.addItem(manualRobots.back().get());

    manualRobots.back().get()->positionUpdate();
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

void Simulator::setActiveRobot(BaseRobot* robot)
{
    if(activeRobot != nullptr)
    {
        activeRobot->setUnselected();
    }

    auto found = std::find(robots.begin(), robots.end(), robot);
    activeRobot = (*found);
    activeRobot->setSelected();
}

void Simulator::setActiveRobot(size_t id)
{
    if(activeRobot != nullptr)
    {
        activeRobot->setUnselected();
    }

    if(id >= 0 && id < robots.size())
    {
        activeRobot = robots.at(id);
        activeRobot->setSelected();
    }
}

void Simulator::setActiveObstacle(size_t id)
{
    if(activeObstacle != nullptr)
    {
        activeObstacle->setUnselected();
    }

    if(id >= 0 && id < obstacles.size())
    {
        activeObstacle = obstacles.at(id).get();
        activeObstacle->setSelected();
    }
}

void Simulator::deleteRobot(size_t id)
{
    if(activeRobot == nullptr)
    {
        return;
    }

    if(! (id >= 0 && id < robots.size()))
    {
        return;
    }

    Rectangle* colliderToDelete = activeRobot->getSim()->getColliderInner();
    auto foundCollider = std::find(colliders.begin(), colliders.end(), colliderToDelete);
    if(foundCollider != colliders.end())
        colliders.erase(foundCollider);


    std::unique_ptr<BaseRobot>* foundRobot;
    auto activeRobotPtr = activeRobot;

    if(typeid(*activeRobot) == typeid(ManualRobot))
    {
        auto foundIt = std::find_if(manualRobots.begin(), manualRobots.end(), [activeRobotPtr](const std::unique_ptr<BaseRobot>& ptr) {
            return ptr.get() == activeRobotPtr;});

        if(foundIt != manualRobots.end())
        {
            foundRobot = &(*foundIt);
            scene.removeItem(foundRobot->get());

            auto foundRob = std::find(robots.begin(), robots.end(), activeRobot);

            robots.erase(foundRob);
            manualRobots.erase(foundIt);
        }
    }
    else
    {
        auto foundIt = std::find_if(autoRobots.begin(), autoRobots.end(), [activeRobotPtr](const std::unique_ptr<BaseRobot>& ptr) {
            return ptr.get() == activeRobotPtr;});

        if(foundIt != autoRobots.end())
        {
            foundRobot = &(*foundIt);
            scene.removeItem(foundRobot->get());

            auto foundRob = std::find(robots.begin(), robots.end(), activeRobot);

            robots.erase(foundRob);
            autoRobots.erase(foundIt);
        }
    }

    activeRobot = nullptr;

    balanceCores();
    scene.update();
}

void Simulator::deleteObstacle(size_t id)
{
    if(activeObstacle == nullptr)
    {
        return;
    }

    if(! (id >= 0 && id < obstacles.size()))
    {
        return;
    }

    Rectangle* colliderToDelete = obstacles.at(id).get()->getSim();
    for(size_t i = id; i < colliders.size(); i++)
    {
        if(colliders.at(i) == colliderToDelete)
        {
            colliders.erase(colliders.begin() + i);
            break;
        }
    }

    scene.removeItem(obstacles.at(id).get());
    obstacles.at(id).release();
    obstacles.erase(obstacles.begin() + id);
    activeObstacle = nullptr;

    scene.update();
}

void Simulator::runSimulation()
{
    this->timerSim.start(timerPeriod);
    this->timerGraphics.start(timerPeriod / 2
                              );

    for(size_t i = 0; i < obstacles.size(); i++)
    {
        obstacles.at(i).get()->setFlag(QGraphicsItem::ItemIsMovable, false);
    }

    for(size_t i = 0; i < robots.size(); i++)
    {
        robots.at(i)->setFlag(QGraphicsItem::ItemIsMovable, false);
    }
}

void Simulator::stopSimulation()
{
    this->timerSim.stop();
    this->timerGraphics.stop();

    for(size_t i = 0; i < obstacles.size(); i++)
    {
        obstacles.at(i).get()->setFlag(QGraphicsItem::ItemIsMovable, true);
    }

    for(size_t i = 0; i < robots.size(); i++)
    {
        robots.at(i)->setFlag(QGraphicsItem::ItemIsMovable, true);
    }
}

ManualRobot* Simulator::getActiveManualRobot()
{
    if(activeRobot == nullptr)
        return nullptr;
    // check if active robot is manul robot, if not return nullptr
    if(typeid(*activeRobot) == typeid(ManualRobot))
        return dynamic_cast<ManualRobot*> (activeRobot);
    else
        return nullptr;
}

