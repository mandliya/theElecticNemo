
#include "random.h"
#include <cstdlib>


float getRand(int min, int max) {
  return min + (rand() / (std::numeric_limits<int>::max()+1.0f))*(max-min);
}

int getRandom(int a, int b) {
  return a + (rand() % b + 1);
}

float getRandFloat(float min, float max) {
    return min + (rand() / (RAND_MAX + 1.0f)) * (max - min);
}

