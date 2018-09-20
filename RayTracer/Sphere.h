#ifndef SPHEREH
#define SPHEREH

#include "Material.h"
#include "Hitable.h"

class Sphere : public Hitable {
public:
	Eigen::Vector3f center;
	float radius;
  Material *mat_ptr;

  Sphere() {};
	Sphere(Eigen::Vector3f cen, float r, Material *m) : center(cen), radius(r), mat_ptr(m) {};
  virtual ~Sphere() { delete mat_ptr; };

	virtual bool hit(const Ray &r, float t_min, float t_max, hit_record &rec) const;
};

#endif //!SPHEREH