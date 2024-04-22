#include "robottest.h"

#include <iostream>

#include "../2DSimulationLib/robot.h"
#include "../2DSimulationLib/rectangle.h"

RobotTest::RobotTest(const std::string &testName) : Test(testName) {}

void RobotTest::initPosition()
{
    testName = "initPosition";
    Robot robot{0, 0, 1, 0, 1};

    assert(0, robot.getX(), "bad x position");

    assert(0, robot.getY(), "bad y position");

    assert( 1, robot.getRadius(), "bad radius value");

    assert(0, robot.getRotation(), "bad rotation value");

    assert( 1, robot.getDetRadius(), "bad detaction radius value");

    Robot robot1{25, 49, 33, 21, 59};

    assert( 25, robot1.getX(), "bad x position");

    assert( 49, robot1.getY(), "bad y position");

    assert(33, robot1.getRadius(), "bad radius value");

    assert(21, robot1.getRotation(), "bad rotation value");

    assert(59, robot1.getDetRadius(), "bad detaction radius value");
}

void RobotTest::simpleMoveForward()
{
    testName = "simpleMoveForward";
    Robot robot{0, 0, 1, 0, 1};
    robot.moveForward(0);

    assert(0, robot.getX(), "bad x position");
    assert(0, robot.getY(), "bad y position");

    robot.moveForward(2);

    assert(2, robot.getX(), "bad x position");
    assert(0, robot.getY(), "bad y position");

    robot.moveForward(160);

    assert(162, robot.getX(), "bad x position");
    assert(0, robot.getY(), "bad y position");


    Robot robot1{1, 10, 1, 90, 1};
    robot1.moveForward(0);

    assert(1, robot1.getX(), "bad x position");
    assert(10, robot1.getY(), "bad y position");

    robot1.moveForward(4);

    assert(1, robot1.getX(), "bad x position");
    assert(14, robot1.getY(), "bad y position");
}

void RobotTest::simpleMoveTo()
{
    testName = "simpleMoveTo";
    Robot robot{0, 0, 1, 0, 1};
    Point p{20, 11};
    robot.moveTo(p);

    assert(20, robot.getX(), "bad x position");
    assert(11, robot.getY(), "bad y position");

    Point p2{4, 20};
    robot.moveTo(p2);

    assert(4, robot.getX(), "bad x position");
    assert(20, robot.getY(), "bad y position");
}

void RobotTest::simpleRotate()
{
    testName = "simpleRotate";
    Robot robot{0, 0, 1, 0, 1};

    assert(0, robot.getX(), "bad x position");
    assert(0, robot.getY(), "bad y position");
    assert(0, robot.getRotation(), "bad rotation position");

    robot.rotate(45);

    assert(0, robot.getX(), "bad x position");
    assert(0, robot.getY(), "bad y position");
    assert(45, robot.getRotation(), "bad rotation position");

    Robot robot1{15, 21, 1, 63, 1};

    assert(15, robot1.getX(), "bad x position");
    assert(21, robot1.getY(), "bad y position");
    assert(63, robot1.getRotation(), "bad rotation position");

    robot1.rotate(14);

    assert(15, robot1.getX(), "bad x position");
    assert(21, robot1.getY(), "bad y position");
    assert(77, robot1.getRotation(), "bad rotation position");

}

void RobotTest::simpleMovement()
{
}

void RobotTest::collisionDetection1()
{
    testName = "collisionDetection1";
    Robot robot1{0, 0, 1, 0, 2};

    std::vector<std::unique_ptr<Rectangle>> obstacles;
    obstacles.push_back(std::make_unique<Rectangle> (1.5, 1.5, 2, 2, 0));
    bool result = robot1.obstacleDetection(obstacles);
    assert(result, false, result, "collision detected when unexpected");

    obstacles.pop_back();

    result = robot1.obstacleDetection(obstacles);
    obstacles.push_back(std::make_unique<Rectangle> (0, 0, 4, 4, 0));
    assert(result, true, result, "collision not detected when expected");
}

bool RobotTest::run()
{

    initPosition();
    simpleMoveForward();
    simpleMoveTo();
    simpleRotate();
    collisionDetection1();
    return failed;
}
