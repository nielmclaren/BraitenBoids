#include "util.hpp"
#include <climits>
#include <cstdlib>
#include <limits>

float Util::randf() {
  return static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
}

float Util::randf(float high) {
  return static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX) * high;
}

float Util::randf(float low, float high) {
  return static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX) *
             (high - low) +
         low;
}