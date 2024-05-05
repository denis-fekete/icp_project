/**
 * @file simulator.h
 * @brief Header file for Simulator class.
 * 
 * Simulator is "interface" between internal calculations, simulation and GUI 
 * (user inputs). It contains vectors of Obstacles and Robots (Manual and 
 * Automatic) and methods needed for their working. Simulator can perform 
 * simulation calculations either on same thread (bad performance useful for
 * small simulations) or by creating multiple threads that are wrapped in 
 * SimulationCore objects, these objects "execute" simulation in another threads 
 * and simulator is "waking" them to "execute" simulation in given intervals.
 * Simulator inherits from QObject, this way simulator can be "connected" to 
 * QTimer that will call its methods in given intervals.
 *
 * @author Denis Fekete (xfeket01@fit.vutbr.cz)
 */

#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>

#include <QObject>
#include <QTimer>
#include <QGraphicsScene>

#include "obstacle.h"
#include "baserobot.h"
#include "autorobot.h"
#include "manualrobot.h"
#include "../2DSimulationLib/rectangle.h"
#include "simulationcore.h"

class Simulator : public QObject
{
    Q_OBJECT
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
     * @brief autoRobots Vector of all robots (automatic or manual)
     */
    std::vector<std::unique_ptr<BaseRobot>> allRobots;

    /**
     * @brief Vector of Robots
     */
    std::vector<std::unique_ptr<Obstacle>> obstacles;

    /**
     * @brief Vector of robot colliders
     */
    std::vector<Robot*> robotColliders;

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
     * @brief Pointer Timer for calling of simulation cycles
     */
    QTimer timerSim;

    /**
     * @brief Pointer Timer for calling of graphics update cycles
     */
    QTimer timerGraphics;

    /**
     * @brief guiUpdateTimer Timer for updating GUI
     */
    QTimer* guiUpdateTimer;

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
     * @brief Last period of cycle timer
     */
    long long cycleTime = 0;

    /**
     * @brief spaceWidth Width of simulation space
     */
    double spaceWidth;

    /**
     * @brief spaceHeight Height of simulation space
     */
    double spaceHeight;

    /**
     * @brief windowWidth Width of the window
     */
    double windowWidth;

    /**
     * @brief windowHeight Height of the window
     */
    double windowHeight;

    /**
     * @brief worldBorder Polygon symbolizing world border
     */
    QGraphicsRectItem worldBorderX;

    /**
     * @brief worldBorder Polygon symbolizing world border
     */
    QGraphicsRectItem worldBorderY;

    /**
     * @brief isPaused Bool value of whenever simulator is running or not
     */
    bool paused;

    /**
     * @brief updateGUI Pointer to the function that updates GUI
     */
    std::function<void()> updateGUI;

    /**
     * @brief smoothConst Constant for smoothing movement
     */
    double smoothConst;
public:
    /**
     * @brief Constructor of Simulator object
     * @param scene Reference to scene for updating it
     * @param maxThreads Maximum allowed threads to use
     * @param timerSim Pointer to QTimer for calling simulation
     * @param timerSim Pointer to QTimer for calling update position of graphics
     * @param width Width of simulation space
     * @param height Height of simulation space
     * @param windowWidth Width of window space
     * @param windowHeight Height of window space
     * @param updateGUIPtr Pointer to function that updates GUI
     * @param Pointer to timer that updates gui,
     * simulator will stop it if simulation stops
     * @param Update period for simulation in milliseconds
     * @param Update period for graphical update in milliseconds
     */
    Simulator(QGraphicsScene &scene,
                                  double width, double height,
                                  double windowWidth, double windowHeight,
                                  std::function<void()> updateGUIPtr,
                                  QTimer* guiUpdateTimer,
                                  int simulationUpdatePeriod,
                                  int graphicsUpdatePeriod);

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
     * @param turnSpeed Turn angle on collision detection
     * @param turnDirection Turn direction of the robot, 1 or -1
     */
    void addAutomaticRobot(double x, double y, double radius, double rot,
                              double detRadius, QColor color, double speed,
                              double turnSpeed, int turnDirection);

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

    /**
     * @return Returns number of robots in simulation
     */
    inline size_t getRobotsCount() { return robots.size(); }

    /**
     * @return Returns number of robots in simulation
     */
    inline size_t getObstaclesCount() { return obstacles.size(); }

    /**
     * @return Returns obstacle at given id
     */
    inline Obstacle* getObstacle(size_t id) { return obstacles.at(id).get(); }

    /**
     * @return Returns robot at given id
     */
    inline BaseRobot* getRobot(size_t id) { return robots.at(id); }

    /**
     * @brief deleteRobot Deletes active robot
     */
    void deleteRobot();

    /**
     * @brief deleteObstacle Deletes active obstacle
     */
    void deleteObstacle();

    /**
     * @brief Starts simulation
     */
    void runSimulation();

    /**
     * @brief Stops simulation
     */
    void stopSimulation();

    /**
     * @return Returns last simulation cycle time
     */
    inline long long getCycleTime() { return this->cycleTime; }

    /**
     * @brief Balances workload between SimulationCores
     */
    void balanceCores();

    /**
     * @brief Sets activeRobot by pointer
     * @param robot Pointer to the robot
     */
    void setActiveRobot(BaseRobot* robot);

    /**
     * @brief Sets activeObstacle by pointer
     * @param robot Pointer to the obstacle
     */
    void setActiveObstacle(Obstacle* obstacle);

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

    /**
     * @return Returns widht of simulation space
     */
    inline double getSimulationWidth() { return spaceWidth; }

    /**
     * @return Returns widht of simulation space
     */
    inline double getSimulationHeight() { return spaceHeight; }

    /**
     * @brief setSimulationSize Sets size of simulation space
     * @param width New width of simulation space
     * @param height New height of simulation space
     */
    void setSimulationSize(double width, double height);

    /**
     * @brief setWindowSize Sets size of window size stored in simulator
     * @param width New value of window width
     * @param height New value of window height
     */
    void setWindowSize(double width, double height);

    /**
     * @brief setBorder Sets border graphical representation
     */
    void setBorder();

    /**
     * @brief unselectActive Unselects active Robot and active obstacle
     */
    void unselectActive();

    /**
     * @brief changeThreadCount Changes number of threads that simulator will use
     * @param threads New number of threads
     */
    void changeThreadCount(int threads);

    /**
     * @return Returns true if Simulator is paused
     */
    inline bool isPaused() { return this->paused; }

    /**
     * @return Returns smooth cosntant of Simulator, used for normalizing/smooting movement
     */
    inline double getSmoothConst() { return this->smoothConst;}

    /**
     * @brief setSimulationTimer Sets simulator update time for simulation
     * @param milliseconds New value in miliseconds
     */
    void setSimulationTimer(int milliseconds);

    /**
     * @brief setGraphicsTimer Sets simulator update time for graphics
     * @param milliseconds New value in miliseconds
     */
    void setGraphicsTimer(int milliseconds);

    /**
     * @return Returns timer period of simulation timer in milliseconds
     */
    int getSimulationTimer();

    /**
     * @return Returns timer period of graphics update timer in milliseconds
     */
    int getGraphicsTimer();

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
                                         std::vector<BaseRobot*>* robots,
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
