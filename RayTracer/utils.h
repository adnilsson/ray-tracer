#ifndef UTILSH
#define UTILSH


#ifndef EIGENH
#define EIGENH
#include <Eigen/Core>
#endif 

namespace utils{

  float randf();

  /**
  * The returned vector originates from the centre of the sphere
  * and is not normalized.
  **/
  Eigen::Vector3f sample_unit_sphere();

  Eigen::Vector3f reflect(const Eigen::Vector3f &v, const Eigen::Vector3f &n);
}


#endif // !UTILSH
