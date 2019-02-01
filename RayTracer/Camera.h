#ifndef CAMERAH
#define CAMERAH

#include "Ray.h"

class Camera {
	public:
		Eigen::Vector3f origin;
		Eigen::Vector3f lower_left_corner;
		Eigen::Vector3f horizontal;
		Eigen::Vector3f vertical;

		Camera(Eigen::Vector3f lookfrom, Eigen::Vector3f lookat, Eigen::Vector3f vup
         , float vfov, float aspect_ratio) { // vfov in degrees
      float theta = vfov * EIGEN_PI/180;
      float half_height = tan(theta / 2);
      float half_width = aspect_ratio * half_height;
      
      Eigen::Vector3f u, v, w; // ortonormal basis vectors 
			origin = lookfrom;
      w = (lookfrom - lookat).normalized(); // remember, the camera looks down -w
      u = vup.cross(w).normalized();
      v = w.cross(u).normalized();

			lower_left_corner = origin - half_width*u - half_height*v - w;
			horizontal = 2*half_width*u;
			vertical = 2*half_height*v;
		};
		~Camera() {};

		Ray get_ray(float u, float v) const {
			return Ray(origin, lower_left_corner + u*horizontal + v*vertical - origin);
		}
};

#endif // !CAMERAH
