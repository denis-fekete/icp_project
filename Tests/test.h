#ifndef TEST_H
#define TEST_H

#include <string>

#include <iostream>

#define DEVIATION_CONST 0.2

class Test
{

protected:
    bool failed;
    std::string testName;

public:
    Test(const std::string &testName);

    /**
     * @brief Runs current tests and returns bool value
     * @return true on success, false on fail
     */
    virtual bool run() = 0;

    std::string getTestName();

    /**
     * @brief Checks condition, if true return false, if false return true and
     * printes message with expected and given values
     * @param cond Condition to be checked
     * @param expected Expecte value
     * @param got Given value
     * @param message Message to be printed in case that conditon if false
     * @return false on success, true on fail
     */
    void assert(bool cond, double expected, double got, const char* message);

    /**
     * @brief Checks if expected is same as got, if not message will be printed
     * @param expected Expecte value
     * @param got Given value
     * @param message Message to be printed in case that conditon if false
     * @return false on success, true on fail
     */
    void Test::assert(double expected, double got, const char* message);

    /**
     * @brief Returns failed attribute
     * @return
     */
    inline bool getFailed() { return failed; }

    inline bool cmpEqual(double a, double b)
    {
        if(a == b)
            return true;
        else if(a - DEVIATION_CONST == b)
            return true;
        else if(a + DEVIATION_CONST == b)
            return true;
        else
            return false;
    }
};

#endif // TEST_H
