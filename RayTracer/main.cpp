/**
* Ordering of includes matters.
* stb_image_write.h must be included first of all, otherwise
* define _CRT_SECURE_NO_WARNINGS does not go through and VS will
* build with errors saying that fopen may be unsafe.
* I do not know what causes this specific behaviour.
**/

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#ifndef EIGENH
#define EIGENH
#include <Eigen/Core>
#include <Eigen/Geometry>
using Eigen::Vector3f;
#endif 

#include "Sphere.h"
#include "HitableList.h"
#include "Camera.h"
#include "Material.h"
#include "utils.h"

typedef uint8_t byte;

/**
* Recursively trace rays with the geometry until background is hit.
* Assumes normalized Ray direction vector.
**/
Vector3f color(const Ray &r, Hitable *world, int depth) {
	hit_record rec;
	if (world -> hit(r, 0.0f + FLT_EPSILON, FLT_MAX, rec)) {
    Ray scatter_ray;
    Vector3f attenuation;
    if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scatter_ray)) {
      return attenuation.cwiseProduct(color(scatter_ray, world, depth + 1));
    }
    
    return Vector3f(0, 0, 0);
	}

	//No intersection. Give background color instead.
	float t = 0.5f*(r.direction().y() + 1.0f);
	return (1.0f - t)*Vector3f(1.0f, 1.0f, 1.0f) + t * Vector3f(0.5f, 0.7f, 1.0f);
}


Hitable *random_scene() {
  int n = 500;
  Hitable **list = new Hitable*[n+1];
  list[0] = new Sphere(Vector3f(0,-1000,0), 1000, new Lambertian(Vector3f(0.5, 0.5, 0.5)));
  
  int i = 1;
  for (int a = -11; a < 11; a++) {
    for (int b = -11; b < 11; b++) {
      float choose_mat = utils::randf();
      Vector3f center(a + 0.9*utils::randf(), 0.2, b + 0.9*utils::randf());
      if ((center - Vector3f(4, 0.2, 0)).norm() > 0.9) {
        if (choose_mat < 0.8) { // choose diffuse material
          list[i++] = new Sphere(center, 0.2, new Lambertian(Vector3f(utils::randf()*utils::randf(), utils::randf()*utils::randf(), utils::randf()*utils::randf())));
        }
        else if (choose_mat < 0.95) { // choose metal material
          list[i++] = new Sphere(center, 0.2, new Metal(Vector3f(0.5*(1 + utils::randf()), 0.5*(1 + utils::randf()), 0.5*(1 + utils::randf())), 0.5*(1 + utils::randf())));
        }
        else { //choose glass material
          list[i++] = new Sphere(center, 0.2, new Dielectric(1.5));
        }
      }
    }
  }

  list[i++] = new Sphere(Vector3f(0, 1, 0), 1.0, new Dielectric(1.5));
  list[i++] = new Sphere(Vector3f(-4, 1, 0), 1.0, new Lambertian(Vector3f(0.4, 0.2, 0.1)));
  list[i++] = new Sphere(Vector3f(4, 1, 0), 1.0, new Metal(Vector3f(0.7, 0.6, 0.5), 0.0));

  return new HitableList(list, i);
}

int main() {
  const int RGB_CHANNELS = 3;
  const int nx = 600;
  const int ny = 400;
  const int ns = 100;
  byte *rgb_image = new byte[nx*ny*RGB_CHANNELS];


  Hitable *world = random_scene();

  Vector3f lookfrom   = Vector3f(13.0f, 2.0f, 3.0f);
  Vector3f lookat     = Vector3f(0.0f, 0.0f, 0.0f);
  float dist_to_focus = 10.0f;
  float aperture      = 0.1f;
  float vfov          = 20.0f;
	Camera cam(lookfrom, lookat, Vector3f(0.0f, 1.0f, 0.0f), vfov
            , static_cast<float>(nx)/ static_cast<float>(ny)
            , aperture, dist_to_focus);

	int i = 0;
	for (int iy = 0; iy < ny; iy++) {
		for (int ix = 0; ix < nx; ix++) {
			Vector3f c(0, 0, 0);
			for (int is = 0; is < ns ; is++) {
				float u = (float(ix) + utils::randf()) / float(nx);
				float v = (float(ny - iy - 1) + utils::randf()) / float(ny);
				Ray r = cam.get_ray(u, v);
				r.normalize_direction();

				c += color(r, world, 0);
			}
			
			c /= float(ns);
			rgb_image[i] = byte(sqrt(c.x()) * 255.99);
			rgb_image[i + 1] = byte(sqrt(c.y()) * 255.99);
			rgb_image[i + 2] = byte(sqrt(c.z()) * 255.99);
			i += RGB_CHANNELS;
		}
	}
	stbi_write_png("ch12.png", nx, ny, RGB_CHANNELS, rgb_image, 0);


	// de-allocation 
	delete[] rgb_image;
	delete world; //owns its list of Hitables and deletes them in its destructor
}
