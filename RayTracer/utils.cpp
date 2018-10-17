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

  Eigen::Vector3f reflect(const Eigen::Vector3f &v, const Eigen::Vector3f &n) {
    return v - 2 * v.dot(n)*n;
  }

  bool refract(const Eigen::Vector3f &v, const Eigen::Vector3f &n, float ni_over_nt, Eigen::Vector3f &refracted) {
    Eigen::Vector3f v_unit = v.normalized();
    /* should be cos(theta_i), but I think this is 
       cos(theta_i + pi/2) = sin(theta_i) since in this 
       implementation v points to the surface, not outwards.
    */
    float dt = v_unit.dot(n);
  
    float discriminant = 1.0f -  ni_over_nt*ni_over_nt * (1.0f - dt*dt); // < 0 if total internal reflection occurs.
    if (discriminant > 0.0f) {
      refracted = ni_over_nt*(v_unit - n*dt) - n*sqrt(discriminant);
      return true;
    }
    return false;
  }

  float schlick(float cosine, float ref_idx) {
    float r0 = (1 - ref_idx) / (1 + ref_idx);
    r0 = r0*r0;
    return r0 + (1 - r0)*powf((1 - cosine), 5);
  }
}