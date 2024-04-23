#ifndef RANDOMGENERATOR_H
#define RANDOMGENERATOR_H

#include <random>

class RandomGenerator {
private:
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_int_distribution<int> dist;

public:
    /**
     * @brief Constructor of RandomGenerator object
     * @param min Minimum value that generator will generate
     * @param max Maximum value that generate will generate
     */
    RandomGenerator(int min, int max) : gen(rd()), dist(min, max) {}

    /**
     * @return Returns pseudo-random value
     */
    inline int getRandomValue() {
        return dist(gen);
    }
};

#endif // RANDOMGENERATOR_H
