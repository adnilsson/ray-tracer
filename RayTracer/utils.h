#ifndef UTILSH
#define UTILSH


#ifndef EIGENH
#define EIGENH
#include <Eigen/Core>
#endif 

namespace utils{
  constexpr auto EPSILON = 1.0e-4;

  float randf();

  /**
  * The returned vector originates from the centre of the sphere
  * and is not normalized.
  **/
  Eigen::Vector4f sample_unit_sphere();
  
  /**
  * Sample the unit square until we get a vector that fits in
  * the unit disk.
  **/
  Eigen::Vector4f sample_unit_disk();


  Eigen::Vector4f reflect(const Eigen::Vector4f &v, const Eigen::Vector4f &n);
  bool refract(const Eigen::Vector4f &v, const Eigen::Vector4f &n, float ni_over_nt, Eigen::Vector4f &refracted);

  float schlick(float cosine, float ref_idx);
}


#endif // !UTILSH
