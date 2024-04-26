#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>

#include <QObject>
#include <QTimer>
#include <QGraphicsScene>

#include "../QtSpecific/baserobot.h"
#include "../QtSpecific/autorobot.h"
#include "../QtSpecific/manualrobot.h"
#include "../QtSpecific/obstacle.h"
#include "../2DSimulationLib/rectangle.h"
#include "simulationcore.h"

class Simulator : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Constructor of Simulator object
     * @param scene Reference to scene for updating it
     * @param maxThreads Maximum allowed threads to use
     * @param timer Pointer to QTimer that will call Simulation
     */
    Simulator(QGraphicsScene &scene, size_t maxThreads, QTimer* timer);
    ~Simulator();

    /*
     * @brief Creates and adds new AutoRobot to the scene
     * @param x Center X position to be set
     * @param y Center Y position to be set
     * @param radius Radius if this Robot
     * @param rot Rotation of this robot
     * @param detRadius Detection radius of this robot
     * @param color Color of the AutoRobot
     * @param speed Speed of the AutoRobot
     * @param turnAngle Turn angle on collision detection
     * @param turnRight Turn direction
     */
    void addAutomaticRobot(double x, double y, double radius, double rot,
                              double detRadius, QColor color, double speed,
                              double turnAngle, bool turnRight);

    /*
     * @brief Creates and adds new AutoRobot to the scene
     * @param x Center X position to be set
     * @param y Center Y position to be set
     * @param radius Radius if this Robot
     * @param rot Rotation of this robot
     * @param detRadius Detection radius of this robot
     * @param color Color of the AutoRobot
     */
    void addManualRobot(double x, double y, double radius, double rot,
                        double detRadius, QColor color);

    /*
     * @brief Creates and adds new Obstacle to the scene
     * @param x Center X position to be set
     * @param y Center Y position to be set
     * @param w Width of the Obstacle
     * @param h Height of the Obstacle
     * @param rot Rotation of this Obstacle
     * @param color Color of the Obstacle
     */
    void addObstacle(double x, double y, double w, double h, double rot,
                            QColor& color);



    /**
     * @brief Initializes SimulationCores and starts it loop
     */
    void initializeCores();


    inline size_t getRobotsCount() { return robots.size(); }

    inline size_t getObstaclesCount() { return obstacles.size(); }

    inline Obstacle* getObstacle(size_t id) { return obstacles.at(id).get(); }
    inline BaseRobot* getRobot(size_t id) { return robots.at(id); }

    void deleteRobot(size_t id);
    void deleteObstacle(size_t id);

    /**
     * @brief Starts simulation
     */
    void runSimulation();

    /**
     * @brief Stops simulation
     */
    void stopSimulation();

    /**
     * @brief Sets period of QTimer updating in milliseconds
     * @param milliSeconds Value to be set to the QTimer
     */
    inline void setTimerPeriod(int milliSeconds) { this->timerPeriod = milliSeconds; }

    /**
     * @return Returns period time of Simulator
     */
    inline double getTimerPeriod() { return this->timerPeriod; }

    /**
     * @return Returns last simulation cycle time
     */
    inline long long getCycleTime() { return this->cycleTime; }

    /**
     * @brief Balances workload between SimulationCores
     */
    void balanceCores();

    /**
     * @brief Sets activeRobot by id
     * @param id Id in vector of AutoRobots
     */
    void setActiveRobot(size_t id);

    /**
     * @brief Sets activeRobot by pointer
     * @param robot Pointer to the robot
     */
    void setActiveRobot(BaseRobot* robot);

    /**
     * @brief Sets activeObstacl by id
     * @param id Id in vector of Obstacles
     */
    void setActiveObstacle(size_t id);

    /**
     * @return Returns pointer to activeRobot
     */
    inline BaseRobot* getActiveRobot() { return activeRobot; }

    /**
     * @return Returns pointer to active ManualRobot, if no active robot is
     * not ManualRobot nullptr will be returned
     */
    ManualRobot* getActiveManualRobot();

    /**
     * @return Returns pointer to activeObstacle
     */
    inline Obstacle* getActiveObstacle() { return activeObstacle; }

private:
    /**
     * @brief Reference to the scene for updating
     */
    QGraphicsScene& scene;

    /**
     * @brief robots Vector of Robots
     */
    std::vector<BaseRobot*> robots;

    /**
     * @brief autoRobots Vector of automatic robots
     */
    std::vector<std::unique_ptr<BaseRobot>> autoRobots;

    /**
     * @brief manualRobots vector of manual robots
     */
    std::vector<std::unique_ptr<BaseRobot>> manualRobots;

    /**
     * @brief Reference to the vector of Robots
     */
    std::vector<std::unique_ptr<Obstacle>> obstacles;

    /**
     * @brief colliders Vector of all colliders
     */
    std::vector<Rectangle*> colliders;

    /**
     * @brief Vector of SimulationCores
     */
    std::vector<std::unique_ptr<SimulationCore>> simCores;

    /**
     * @brief Vector of threads that run SimulationCores
     */
    std::vector<std::unique_ptr<std::thread>> simThreads;

    /**
     * @brief activeRobot Pointer to active AutoRobot
     */
    BaseRobot* activeRobot;

    /**
     * @brief activeObstacle Pointer to active active Obstacle
     */
    Obstacle* activeObstacle;

    /**
     * @brief Pointer to global timer
     */
    QTimer* timer;

    /**
     * @brief Mutex for SimulationCore synchronization
     */
    std::mutex mutex;

    /**
     * @brief Condition for waking SimulationCores
     */
    std::condition_variable wakeCores;

    /**
     * @brief Bool value for keeping SimulationCores running
     */
    bool keepSimCoresRunning;

    /**
     * @brief Number of maximum threads
     */
    size_t maxThreads;

    /**
     * @brief Timer period in milliseconds
     */
    int timerPeriod;

    /**
     * @brief Last period of cycle timer
     */
    long long cycleTime = 0;

    /**
     * @brief Creates SimulationCore in another thread and runs in main loop
     * for simulation
     * @param simCores Reference to vector of SimulationCores
     * @param robots Reference to vector of AutoRobots
     * @param wakeCores Pointer to condition variable for waking SimulationCore
     * @param mutex Pointer to mutex for synchronizing SimulationCore
     * @param keepSimulating Pointer to bool value for turning off simulation loop
     */
    static void createSimulationCore(std::vector<std::unique_ptr<SimulationCore>>* simCores,
                                         std::vector<BaseRobot *> *robots,
                                         std::condition_variable* wakeCores,
                                         std::mutex* mutex,
                                         bool* keepSimulating);
protected slots:

    /**
     * @brief Function that calls all SimulationCores to simulate once
     */
    void simulationCycle();
};

#endif // SIMULATOR_H
