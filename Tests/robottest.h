#ifndef ROBOTTEST_H
#define ROBOTTEST_H

#include "test.h"

class RobotTest : public Test
{
public:
    RobotTest(const std::string &testName);

    void initPosition();
    void simpleMoveForward();
    void simpleMoveTo();
    void simpleRotate();
    void simpleMovement();
    void collisionDetection1();
    void collisionDetection2();

    bool run() override;
};

#endif // ROBOTTEST_H
