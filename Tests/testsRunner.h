#ifndef TESTSRUNNER_H
#define TESTSRUNNER_H

#include <Tests/test.h>

#include "robottest.h"

#include <vector>
#include <iostream>

class TestRunner
{
public:
    static void testRunner()
    {
        std::vector<Test*> tests;
        tests.push_back(new RobotTest("Robot test"));
        unsigned int failed = 0;
        unsigned int successful = 0;
        bool result;

        for(Test* test : tests)
        {
            std::cout << "Test" << test->getTestName() << std::endl;
            result = test->run();
            if(result)
                successful++;
            else
                failed++;

            std::cout << "Test result: " << ((result)? "Successful" : "Failed") << std::endl;
        }

        std::cout << "------------------------------" << std::endl;
        std::cout << "Results: " << std::endl;

        std::cout << "All tests: " << failed + successful;
        std::cout << ", percentage= " << double{(100.0 * successful ) / ((failed > 0)? failed : 1)} << "%" << std::endl;

        std::cout << "Failed= " << failed << "/ Successful= " << successful << ::std::endl;

    }
};

#endif // TESTSRUNNER_H
