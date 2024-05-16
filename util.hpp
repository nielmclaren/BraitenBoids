#pragma once

/**
 * Convenience functions.
 *
 * For random number generators, you'll need to initialize the seed yourself.
 */
class Util {
public:
  static float randf();
  static float randf(float high);
  static float randf(float low, float high);
};