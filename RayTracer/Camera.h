#ifndef CAMERAH
#define CAMERAH

#include "Ray.h"

#ifndef UTILSH
#include "utils.h"
#endif // !UTILSH


class Camera {
	public:
		Eigen::Vector4f origin;
		Eigen::Vector4f lower_left_corner;
		Eigen::Vector4f horizontal;
		Eigen::Vector4f vertical;
    Eigen::Vector4f u, v, w; // ortonormal basis vectors 
    float lens_radius;

		Camera(Eigen::Vector4f lookfrom, Eigen::Vector4f lookat, Eigen::Vector4f vup
         , float vfov, float aspect_ratio, float aperture, float focus_dist) {
      lens_radius = aperture / 2;
      float theta = vfov * EIGEN_PI/180; // vfov in degrees
      float half_height = tan(theta / 2);
      float half_width = aspect_ratio * half_height;
      
			origin = lookfrom;
      w = (lookfrom - lookat).normalized(); // remember, the camera looks down -w
      u = vup.cross3(w).normalized();
      v = w.cross3(u).normalized();

			lower_left_corner = origin + focus_dist*(-half_width*u - half_height*v - w);
			horizontal = 2*half_width*focus_dist*u;
			vertical = 2*half_height*focus_dist*v;
		};
		~Camera() {};

		Ray get_ray(float s, float t) const {
      Eigen::Vector4f rd = lens_radius * utils::sample_unit_disk();
      Eigen::Vector4f offset = u*rd.x()  + v*rd.y();
			return Ray(origin + offset, lower_left_corner + s*horizontal + t*vertical - origin - offset);
		}
};

#endif // !CAMERAH
