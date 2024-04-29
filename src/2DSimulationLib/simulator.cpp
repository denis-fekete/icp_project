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
    connect(&timerGraphics, SIGNAL(timeout()), &scene, SLOT(advance()));
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
        // for(size_t index = 0; index < robots.size(); index++)
        // {
        //     robots.at(index)->simulate();
        //     robots.at(index)->positionUpdate();
        // }
        return;

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
                                     double turnAngle, int turnDirection)
{
    // const for normalizing values
    const double timeNorm = 1000 / timerPeriod;

    // create new AutoRobot and add it to vector of AutoRobots
    autoRobots.push_back(std::make_unique<AutoRobot> (x, y, radius, rot,
                                                     detRadius, color,
                                                     speed / timeNorm,
                                                     turnAngle / timeNorm,
                                                     turnDirection, &colliders, &robotColliders, this));

    auto addedRobot = autoRobots.back().get();
    // add robot to the vector of robotColliders
    robotColliders.push_back(addedRobot->getSim());

    // add it do vector of all robots
    robots.push_back(addedRobot);

    // initialize it and add it the scene
    addedRobot->initialize();
    scene.addItem(addedRobot);

    addedRobot->positionUpdate();

    balanceCores();
    scene.update();
}

void Simulator::addManualRobot(double x, double y, double radius, double rot,
                                double detRadius, QColor color)
{
    // create new AutoRobot and add it to vector of manual robots
    manualRobots.push_back(std::make_unique<ManualRobot> (x, y, radius, rot, detRadius, color, &colliders, &robotColliders, this));

    auto addedRobot = manualRobots.back().get();
    // add robot to the vector of robotColliders
    robotColliders.push_back(addedRobot->getSim());

    // add it do vector of all robots
    robots.push_back(addedRobot);

    // initialize it and add it the scene
    addedRobot->initialize();
    scene.addItem(addedRobot);

    addedRobot->positionUpdate();
    balanceCores();
    scene.update();
}

void Simulator::addObstacle(double x, double y, double w, double h, double rot,
                                  QColor& color)
{
    obstacles.push_back(std::make_unique<Obstacle>(x, y, w, h, rot, color, this));
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

    // auto found = std::find(robots.begin(), robots.end(), robot);
    // activeRobot = (*found);
    activeRobot = robot;
    activeRobot->setSelected();
}

void Simulator::setActiveRobot(size_t id)
{
    if(activeRobot != nullptr)
    {
        activeRobot->setUnselected();
    }

    if(id < robots.size())
    {
        activeRobot = robots.at(id);
        activeRobot->setSelected();
    }
}

void Simulator::setActiveObstacle(Obstacle* obstacle)
{
    if(activeObstacle != nullptr)
    {
        activeObstacle->setUnselected();
    }

    activeObstacle = obstacle;
    activeObstacle->setSelected();
}

void Simulator::setActiveObstacle(size_t id)
{
    if(activeObstacle != nullptr)
    {
        activeObstacle->setUnselected();
    }

    if(id < obstacles.size())
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

    if(! (id < robots.size()))
    {
        return;
    }

    // delete robot from robotColliders vector
    Robot* colliderToDelete = activeRobot->getSim();
    auto foundCollider = std::find(robotColliders.begin(), robotColliders.end(), colliderToDelete);
    if(foundCollider != robotColliders.end())
        robotColliders.erase(foundCollider);


    std::unique_ptr<BaseRobot>* foundRobot;
    auto activeRobotPtr = activeRobot;
    // remove robot from baseRobot (all) and manualRobot
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
    else // remove robot from baseRobot (all) and autoRobot
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

    if(!(id < obstacles.size()))
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
    for(size_t i = 0; i < obstacles.size(); i++)
    {
        obstacles.at(i).get()->setFlag(QGraphicsItem::ItemIsMovable, false);
    }

    for(size_t i = 0; i < robots.size(); i++)
    {
        robots.at(i)->setFlag(QGraphicsItem::ItemIsMovable, false);
    }

    this->timerSim.start(timerPeriod);
    this->timerGraphics.start(timerPeriod / 2);
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
    // check if active robot is manual robot, if not return nullptr
    if(typeid(*activeRobot) == typeid(ManualRobot))
        return dynamic_cast<ManualRobot*> (activeRobot);
    else
        return nullptr;
}

