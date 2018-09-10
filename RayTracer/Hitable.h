#ifndef HITABLE
#define HITABLE

#include "Ray.h"

struct hit_record {
	float t;
	Eigen::Vector3f p;
	Eigen::Vector3f normal;
};


class Hitable {
	public:
		virtual bool hit(const Ray& r, float t_min, float t_max, hit_record &rec) const = 0;
};

#endif // !HITABLE
