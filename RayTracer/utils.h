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
}


#endif // !UTILSH
