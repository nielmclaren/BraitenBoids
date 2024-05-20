#pragma once

/**
 * Convenience functions.
 *
 * For random number generators, you'll need to initialize the seed yourself.
 */
class Util {
public:
  static const float pi;

  static float linearInterp(float v, float inLow, float inHigh, float outLow,
                            float outHigh);
  static float randf();
  static float randf(float high);
  static float randf(float low, float high);
};