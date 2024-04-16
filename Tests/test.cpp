#include "test.h"
#include <iostream>
#include <utility>

Test::Test(const std::string &testName) : testName(testName)
{
    this->testName = testName;
    this->failed = false;
}


std::string Test::getTestName()
{
    return this->testName;
}

void Test::assert(bool cond, double expected, double got, const char* message)
{
    if(!cond)
    {
        std::cout << "\t" << testName << " failed: " <<  message <<
            ". Expected=" << expected <<
            ", Got: " << got <<
            std::endl;

        this->failed = true;
    }    
}

void Test::assert(double expected, double got, const char* message)
{
    if(!cmpEqual(expected, got))
    {
        std::cout << "\t" << testName << " failed: " <<  message <<
            ". Expected=" << expected <<
            ", Got: " << got <<
            std::endl;

        this->failed = true;
    }
}
