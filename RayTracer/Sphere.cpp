#include "Sphere.h"

/**
* Follows the test described by Real Time Rendering 3rd-edition.
* Assumes that Ray r has a normalized direction vector.
* Side-effect: places the most positive t in isect_t if intersection occurs.
* TODO: change to the optimized test.
*/
bool Sphere::hit(const Ray &r, float t_min, float t_max, hit_record &rec) const {
	Eigen::Vector3f oc = r.origin() - center;

	float c = oc.dot(oc) - pow(radius, 2);
	float b = r.direction().dot(oc);
	float discriminant = pow(b, 2) - c;

	// Gives true if distcriminant = 0.0f
	if (discriminant + FLT_EPSILON > 0.0f) {
		float inv_rad = 1 / radius;
		
		float temp = -b - sqrt(discriminant);
		if (temp < t_max && temp > t_min) {
			rec.t = temp;
			rec.p = r.point_at_parameter(temp);
			rec.normal = (rec.p - center) *inv_rad;
			return true;
		}

		temp = -b + sqrt(discriminant);
		if (temp < t_max && temp > t_min) {
			rec.t = temp;
			rec.p = r.point_at_parameter(temp);
			rec.normal = (rec.p - center) *inv_rad;
			return true;
		}
	}
	return false;
}