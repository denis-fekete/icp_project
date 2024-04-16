#ifndef RANDOMGENERATOR_H
#define RANDOMGENERATOR_H

#include <random>

class RandomGenerator {
private:
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_int_distribution<int> dist;

public:
    RandomGenerator(int min, int max) : gen(rd()), dist(min, max) {}

    inline int getRandomValue() {
        return dist(gen);
    }
};


#endif // RANDOMGENERATOR_H


