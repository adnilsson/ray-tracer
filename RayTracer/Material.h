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
    virtual bool scatter(const Ray &r_in, const hit_record &rec, Eigen::Vector4f &attenuation, Ray &scattered) const = 0;
};


class Lambertian : public Material {
  public: 
    Eigen::Vector4f albedo; // "reflectiveness" 

    Lambertian(const Eigen::Vector4f a) : albedo(a) {}
    virtual ~Lambertian() {}
    
    /**
    * Always scatter in a random direction and attenuate by albedo.
    **/
    virtual bool scatter(const Ray &r_in, const hit_record &rec, Eigen::Vector4f &attenuation, Ray &scattered) const{
      Eigen::Vector4f target = rec.p + rec.normal + utils::sample_unit_sphere();
      scattered = Ray(rec.p, (target - rec.p).normalized());
      attenuation = albedo;
      return true;
    }
};


class Metal : public Material {
  public:
    Eigen::Vector4f albedo;
    float fuzz;

    Metal(const Eigen::Vector4f a, float f) : albedo(a) { fuzz =  f < 1.0f ? f : 1.0f; }

    virtual bool scatter(const Ray &r_in, const hit_record &rec, Eigen::Vector4f &attenuation, Ray &scattered) const {
      Eigen::Vector4f reflected = utils::reflect(r_in.direction(), rec.normal);
      Eigen::Vector4f scatter_dir = reflected + fuzz*utils::sample_unit_sphere();
      scatter_dir.normalize();

      scattered = Ray(rec.p, scatter_dir);
      attenuation = albedo;
      return (scattered.direction().dot(rec.normal) > 0.0f + FLT_EPSILON);
    }
};


class Dielectric : public Material {
public:
  float refractive_index;

  Dielectric(float ri) : refractive_index(ri) { }

  virtual bool scatter(const Ray &r_in, const hit_record &rec, Eigen::Vector4f &attenuation, Ray &scattered) const {
    Eigen::Vector4f outward_normal, refracted, reflected;
    attenuation = Eigen::Vector4f(1.0f, 1.0f, 1.0f, 0.0f);
    float ni_over_nt, reflect_prob, cosine;

    if (r_in.direction().dot(rec.normal) > 0.0f + FLT_EPSILON) {
      // refract from this medium to air 
      outward_normal = -rec.normal;
      ni_over_nt = refractive_index; // (refractive_index / 1.0)
      cosine = refractive_index * r_in.direction().dot(rec.normal) / r_in.direction().norm();
    }
    else {
      // refract from air into this medium
      outward_normal = rec.normal;
      ni_over_nt = 1.0f / refractive_index; 
      cosine = -(r_in.direction().dot(rec.normal) / r_in.direction().norm());
    }

    if (utils::refract(r_in.direction(), outward_normal, ni_over_nt, refracted)) {
      reflect_prob = utils::schlick(cosine, refractive_index);
    }
    else {
      reflect_prob = 1.0f;
    }

    if (utils::randf() < reflect_prob) {
      reflected = utils::reflect(r_in.direction(), rec.normal);
      reflected.normalize();
      scattered = Ray(rec.p, reflected);
    }
    else {
      refracted.normalize();
      scattered = Ray(rec.p, refracted);
    }
    return true;
  }
};

#endif // !MATERIALH