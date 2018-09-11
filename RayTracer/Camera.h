#ifndef CAMERAH
#define CAMERAH

#include "Ray.h"

class Camera {
	public:
		Eigen::Vector3f origin;
		Eigen::Vector3f lower_left_corner;
		Eigen::Vector3f horizontal;
		Eigen::Vector3f vertical;

		Camera() {
			origin = Eigen::Vector3f(0.0f, 0.0f, 0.0f);
			lower_left_corner = Eigen::Vector3f(-2.0f, -1.0f, -1.0f);
			horizontal = Eigen::Vector3f(4.0f, 0.0f, 0.0f);
			vertical = Eigen::Vector3f(0.0f, 2.0f, 0.0f);
		};
		~Camera() {};

		Ray get_ray(float u, float v) const {
			return Ray(origin, lower_left_corner + u*horizontal + v*vertical - origin);
		}
};

#endif // !CAMERAH
