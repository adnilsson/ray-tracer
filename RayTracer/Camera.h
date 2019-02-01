#ifndef CAMERAH
#define CAMERAH

#include "Ray.h"

class Camera {
	public:
		Eigen::Vector3f origin;
		Eigen::Vector3f lower_left_corner;
		Eigen::Vector3f horizontal;
		Eigen::Vector3f vertical;

		Camera(float vfov, float aspect_ratio) { // vfov in degrees
      float theta = vfov * EIGEN_PI/180;
      float half_height = tan(theta / 2);
      float half_width = aspect_ratio * half_height;

			origin = Eigen::Vector3f(0.0f, 0.0f, 0.0f);
			lower_left_corner = Eigen::Vector3f(-half_width, -half_height, -1.0f);
			horizontal = Eigen::Vector3f(2*half_width, 0.0f, 0.0f);
			vertical = Eigen::Vector3f(0.0f, 2.0f*half_height, 0.0f);
		};
		~Camera() {};

		Ray get_ray(float u, float v) const {
			return Ray(origin, lower_left_corner + u*horizontal + v*vertical - origin);
		}
};

#endif // !CAMERAH
