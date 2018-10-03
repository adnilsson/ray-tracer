#ifndef MATERIALH
#define MATERIALH

struct hit_record;

#include "Ray.h"
#include "Hitable.h"
#include "utils.h"

#ifndef EIGENH
#define EIGENH
#include <Eigen/Core>
#endif 

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


class Metal : public Material {
  public:
    Eigen::Vector3f albedo;
    float fuzz;

    Metal(const Eigen::Vector3f a, float f) : albedo(a) { fuzz =  f < 1.0f ? f : 1.0f; }

    virtual bool scatter(const Ray &r_in, const hit_record &rec, Eigen::Vector3f &attenuation, Ray &scattered) const {
      Eigen::Vector3f reflected = utils::reflect(r_in.direction(), rec.normal);
      scattered = Ray(rec.p, reflected + fuzz*utils::sample_unit_sphere());
      attenuation = albedo;
      return (scattered.direction().dot(rec.normal) > 0.0f + FLT_EPSILON);
    }
};

#endif // !MATERIALH