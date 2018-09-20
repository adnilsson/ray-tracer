#ifndef MATERIALH
#define MATERIALH

struct hit_record;

#include "Ray.h"
#include "Hitable.h"
#include "utils.h"

class Material {
  public:
    virtual bool scatter(const Ray &r_in, const hit_record &rec, Eigen::Vector3f &attenuation, Ray &scattered) const = 0;
};


class Lambertian : public Material {
  public: 
    Eigen::Vector3f albedo; // "reflectiveness" 

    Lambertian(const Eigen::Vector3f a) : albedo(a) {}
    virtual ~Lambertian() {}
    /**
    * Always scatter in a random direction and attenuate by albedo.
    **/
    virtual bool scatter(const Ray &r_in, const hit_record &rec, Eigen::Vector3f &attenuation, Ray &scattered) const{
      Eigen::Vector3f target = rec.p + rec.normal + utils::sample_unit_sphere();
      scattered = Ray(rec.p, (target - rec.p).normalized());
      attenuation = albedo;
      return true;
    }
};


#endif // !MATERIALH