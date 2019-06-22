#include "Sphere.h"

/**
* Follows the test described by Real Time Rendering 3rd-edition.
* Assumes that Ray r has a normalized direction vector.
* Side-effect: places the most positive t in isect_t if intersection occurs.
*
* Returns true if the ray intersects the sphere and records intersection info
* (vector to point of intersetion, material) in rec.
*/
bool Sphere::hit(const Ray &r, float t_min, float t_max, hit_record &rec) const {
	Eigen::Vector3f oc =  center - r.origin();
  
  float s  = oc.dot(r.direction());
  float oc_squared_len = oc.dot(oc);
  float radius_sqaured = radius * radius;

  // Reject if ray origin is behind the sphere center and 
  // more than a radius distance away
  if (s < 0.0f && oc_squared_len > radius_sqaured) {
    return false;
  }

  // Reject if the ray will miss the sphere
  float m_squared = oc_squared_len - s * s;
  if (m_squared > radius_sqaured) return false;

  // We now know that the ray intersects the sphere.

  float q = sqrt(radius_sqaured - m_squared);
  float t;
  if (oc_squared_len > radius_sqaured) {
    t = s - q;
  }
  else {
    t = s + q;
  }

  if (t < t_min || t > t_max) {
    return false;
  }

  rec.t = t;
  rec.p = r.point_at_parameter(t);
  rec.normal = (rec.p - center) / radius;
  rec.mat_ptr = mat_ptr;
  return true;
}