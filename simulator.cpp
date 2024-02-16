#include "simulator.h"

Simulator::Simulator(std::vector<AutoRobot*>* robots, std::vector<Obstacle*>* obstacles, int numOfRobotsPerThread)
{
    this->robots = robots;
    this->obstacles = obstacles;
    this->numOfRobotsPerThread = numOfRobotsPerThread;

    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(SimulationCycle()));
}

void Simulator::SimulationCycle()
{

}


void Simulator::SimulateRobots()
{

    //TODO: span vector in argument of function
}

void Simulator::RunSimulation()
{
    this->timer->start(timerPeriod_ms);
}

void Simulator::StopSimulation()
{
    this->timer->stop();
}

void Simulator::SetTimerPeriod(int milliSeconds)
{
    this->timerPeriod_ms = milliSeconds;
}
