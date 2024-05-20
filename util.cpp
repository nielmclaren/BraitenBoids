#include "util.hpp"
#include <climits>
#include <cmath>
#include <cstdlib>
#include <limits>

const float Util::pi = std::acos(-1.0f);

float Util::linearInterp(float v, float inLow, float inHigh, float outLow,
                         float outHigh) {
  return outLow + (v - inLow) * (outHigh - outLow) / (inHigh - inLow);
}

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