#ifndef SPHEREH
#define SPHEREH

#include "Hitable.h"

class Sphere : public Hitable {
public:
	Eigen::Vector3f center;
	float radius;

	Sphere();
	Sphere(Eigen::Vector3f cen, float r) : center(cen), radius(r) {};
	virtual ~Sphere() {};

	virtual bool hit(const Ray &r, float t_min, float t_max, hit_record &rec) const;
};

#endif //!SPHEREH