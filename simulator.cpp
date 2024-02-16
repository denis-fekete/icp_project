#include "simulator.h"

Simulator::Simulator(std::vector<AutoRobot*>* robots, std::vector<Obstacle*>* obstacles, int numOfRobotsPerThread)
{
    this->robots = robots;
    this->obstacles = obstacles;
    this->numOfRobotsPerThread = numOfRobotsPerThread;

    this->timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(SimulationCycle()));
    // this->timer->start(this->numOfRobotsPerThread);
}


void Simulator::SimulationCycle()
{
    std::vector<std::thread*> simThreads;

    const size_t vectorMaxSize = robots->size();

    // Split simulation calculating between threads
    for(size_t i = 0; i < vectorMaxSize; i++)
    {

        size_t end = ( (i + 1) * numOfRobotsPerThread ) - 1;
        end = (end > vectorMaxSize)? vectorMaxSize : end;

        simThreads.push_back(new std::thread(SimulateGroup,
                                             robots,
                                             i * numOfRobotsPerThread,
                                             end
                                             ));
    }

    // Wait for all threads to end
    for(size_t i = 0; i < simThreads.size(); i++)
    {
        simThreads.at(i)->join();
    }
}


void Simulator::SimulateGroup(std::vector<AutoRobot*>* robots, const size_t start, const size_t end)
{
    for(size_t i = 0; start + i < end; i++)
    {
        robots->at(start+i)->Simulate();
    }
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
