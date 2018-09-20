#include "utils.h"

#include <random>

namespace utils {
  // Globals
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<float> dist(0.0f, 1.0f);

  float randf() {
    return dist(gen);
  }

  Eigen::Vector3f sample_unit_sphere() {
    Eigen::Vector3f p;
    do {
      p = 2.0f * Eigen::Vector3f(randf(), randf(), randf()) - Eigen::Vector3f(1, 1, 1);
    } while (p.squaredNorm() >= 1.0f);
    return p;
  }
}