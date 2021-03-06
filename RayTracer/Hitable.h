#ifndef HITABLE
#define HITABLE

#include "Ray.h"

class Material;

struct hit_record {
	float t;
	Eigen::Vector4f p;
	Eigen::Vector4f normal;
  Material *mat_ptr;
};


class Hitable {
	public:
		virtual bool hit(const Ray& r, float t_min, float t_max, hit_record &rec) const = 0;
};

#endif // !HITABLE
