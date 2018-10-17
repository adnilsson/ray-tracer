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


int main() {
  const int RGB_CHANNELS = 3;
  const int nx = 200;
  const int ny = 100;
  const int ns = 1000;
  byte *rgb_image = new byte[nx*ny*RGB_CHANNELS];


  Vector3f lower_left_corner(-2.0f, -1.0f, -1.0f);
  Vector3f origin(0.0f, 0.0f, 0.0f); // center of the image plane

  // Vectors that span the whole image plane
  Vector3f horizontal(4.0f, 0.0f, 0.0f);
  Vector3f vertical(0.0f, 2.0f, 0.0f);

  const unsigned int n_hitables = 5;
  Hitable *list[n_hitables];
  list[0] = new Sphere(Vector3f(0, 0, -1), 0.5, new Lambertian(Vector3f(0.1f, 0.2f, 0.5f)));
  list[1] = new Sphere(Vector3f(0, -100.5, -1), 100, new Lambertian(Vector3f(0.8f, 0.8f, 0.0f)));
  list[2] = new Sphere(Vector3f(1, 0, -1), 0.5, new Metal(Vector3f(0.8f, 0.6f, 0.2f), 0));
  list[3] = new Sphere(Vector3f(-1, 0, -1), 0.5, new Dielectric(1.5));
  list[4] = new Sphere(Vector3f(-1, 0, -1), -0.45, new Dielectric(1.5));
	HitableList *world = new HitableList(list, n_hitables);

	Camera cam;

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
	stbi_write_png("ch9.2.png", nx, ny, RGB_CHANNELS, rgb_image, 0);


	// de-allocation 
	delete[] rgb_image;
	delete world; //owns its list of Hitables and deletes them in its destructor
}
