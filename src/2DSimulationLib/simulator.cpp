#include "simulator.h"

#include <algorithm>


Simulator::Simulator(QGraphicsScene &scene,
                     double width, double height,
                     double windowWidth, double windowHeight):
                    scene(scene)
{
    timerPeriod = 30; // default 30 ms period
    activeRobot = nullptr;
    activeObstacle = nullptr;
    keepSimCoresRunning = false;

    this->spaceWidth = width;
    this->spaceHeight = height;

    this->windowWidth = windowWidth;
    this->windowHeight= windowHeight;

    maxThreads = 0;

    connect(&timerSim, SIGNAL(timeout()), this, SLOT(simulationCycle()));
    connect(&timerGraphics, SIGNAL(timeout()), &scene, SLOT(advance()));

    scene.addItem(&worldBorderX);
    scene.addItem(&worldBorderY);
}

Simulator::~Simulator()
{
    this->keepSimCoresRunning = false;
    wakeCores.notify_all();

    this->timerSim.stop();
}

void Simulator::simulationCycle()
{
    if(robots.size() == 0)
    {
        return;
    }

    if(maxThreads == 0)
    {
        for(size_t i = 0; i < robots.size(); i++)
        {
            robots.at(i)->simulate();
        }
    }
    else
    {
        wakeCores.notify_all();
    }

    scene.update();
}

void Simulator::initializeCores()
{
    // initialize cores
    keepSimCoresRunning = true;
    if(maxThreads > 0)
    {
        for(size_t i = 0; i < maxThreads; i++)
        {
            simThreads.push_back(std::make_unique<std::thread> (&Simulator::createSimulationCore, &simCores, &robots, &wakeCores, &mutex, &keepSimCoresRunning));
            using namespace std::chrono_literals;
            std::this_thread::sleep_for(500ms);
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




void Simulator::balanceCores()
{
    if(maxThreads == 0)
    {
        return;
    }
    if(simCores.size() < maxThreads)
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
    allRobots.push_back(std::make_unique<AutoRobot> (x, y, radius, rot,
                                                     detRadius, color,
                                                     speed / timeNorm,
                                                     turnAngle / timeNorm,
                                                     turnDirection, &colliders, &robotColliders,
                                                     this, &spaceWidth, &spaceHeight));

    auto addedRobot = allRobots.back().get();
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
    allRobots.push_back(std::make_unique<ManualRobot> (x, y, radius, rot,
                                                         detRadius, color, &colliders, &robotColliders,
                                                         this,  &spaceWidth, &spaceHeight));

    auto addedRobot = allRobots.back().get();
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

    activeRobot = robot;
    activeRobot->setSelected();
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

void Simulator::deleteRobot()
{
    if(activeRobot == nullptr)
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

    // find robot it vector of robots
    auto foundIt = std::find_if(allRobots.begin(), allRobots.end(),
        [activeRobotPtr](const std::unique_ptr<BaseRobot>& ptr) { return ptr.get() == activeRobotPtr; });

    // if found
    if(foundIt != allRobots.end())
    {
        foundRobot = &(*foundIt);
        scene.removeItem(foundRobot->get());

        auto foundRob = std::find(robots.begin(), robots.end(), activeRobot);

        robots.erase(foundRob);
        allRobots.erase(foundIt);
    }

    activeRobot = nullptr;

    balanceCores();
    scene.update();
}

void Simulator::deleteObstacle()
{
    if(activeObstacle == nullptr)
    {
        return;
    }

    // find active obstacle in colliders
    auto itColliders = std::find(colliders.begin(), colliders.end(), activeObstacle->getSim());

    // erase it from colliders
    colliders.erase(itColliders);

    // find active obstacle in obstacles
    auto obstaclePtr = activeObstacle;
    auto itObstacles = std::find_if(obstacles.begin(), obstacles.end(),
        [obstaclePtr](const std::unique_ptr<Obstacle>& ptr) { return ptr.get() == obstaclePtr; });

    // remove obstacle from the scene
    scene.removeItem(activeObstacle);
    // release unique_ptr
    itObstacles->release();
    // delete found obstacle
    obstacles.erase(itObstacles);

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

void Simulator::setSimulationSize(double width, double height)
{
    this->spaceWidth = width;
    this->spaceHeight = height;

    setBorder();
}

void Simulator::setWindowSize(double width, double height)
{
    this->windowWidth = width;
    this->windowHeight = height;

    setBorder();
}

void Simulator::setBorder()
{
    worldBorderX.setBrush(QBrush(Qt::gray, Qt::DiagCrossPattern));
    worldBorderY.setBrush(QBrush(Qt::gray, Qt::DiagCrossPattern));

    worldBorderX.setPen(QPen(Qt::NoPen));
    worldBorderY.setPen(QPen(Qt::NoPen));

    worldBorderX.setRect(spaceWidth, 0, std::max(windowWidth-spaceWidth, 0.0), std::max(windowHeight, spaceHeight));
    worldBorderY.setRect(0, spaceHeight, spaceWidth, std::max(windowHeight-spaceHeight, 0.0));

    scene.update();
}

void Simulator::unselectActive()
{
    if(activeObstacle != nullptr)
    {
        activeObstacle->setUnselected();
        activeObstacle = nullptr;
    }

    if(activeRobot != nullptr)
    {
        activeRobot->setUnselected();
        activeRobot = nullptr;
    }
}

void Simulator::changeThreadCount(int threads)
{
    this->maxThreads = threads;

    initializeCores();
    balanceCores();
}
