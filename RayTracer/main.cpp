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

#include <random>
#include "Sphere.h"
#include "HitableList.h"
#include "Camera.h"

typedef uint8_t byte;

/**
* Gives a color that is linerarly interpolated between white and 
* blue-ish based on the direction of the ray.
* Assumes normalized Ray direction vector.
**/
Vector3f color(const Ray &r, Hitable *world) {
	hit_record rec;
	if (world -> hit(r, 0.0, FLT_MAX, rec)) {
		return 0.5*Vector3f(rec.normal.x()+1, rec.normal.y()+1, rec.normal.z()+1);
	}

	//No intersection. Give background color instead
	float t = 0.5f*(r.direction().y() + 1.0f);
	return (1.0f - t)*Vector3f(1.0f, 1.0f, 1.0f) + t * Vector3f(0.5f, 0.7f, 1.0f);
}


int main() {
	const int RGB_CHANNELS = 3;
	const int nx = 200;
	const int ny = 100;
	const int ns = 100;
	byte *rgb_image = new byte[nx*ny*RGB_CHANNELS];

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dist(0.0f, 1.0f);

	Vector3f lower_left_corner(-2.0f, -1.0f, -1.0f);
	Vector3f origin(0.0f, 0.0f, 0.0f); // center of the image plane

	// Vectors that span the whole image plane
	Vector3f horizontal(4.0f, 0.0f, 0.0f);
	Vector3f vertical(0.0f, 2.0f, 0.0f);

	const unsigned int n_hitables = 2;
	Hitable *list[n_hitables];
	list[0] = new Sphere(Vector3f(0, 0, -1), 0.5);
	list[1] = new Sphere(Vector3f(0, -100.5, -1), 100);
	HitableList *world = new HitableList(list, n_hitables);

	Camera cam;

	int i = 0;
	for (int iy = 0; iy < ny; iy++) {
		for (int ix = 0; ix < nx; ix++) {
			Vector3f c(0, 0, 0);
			for (int is = 0; is < ns ; is++) {
				float u = (float(ix) + dist(gen)) / float(nx);
				float v = (float(ny - iy - 1) + dist(gen)) / float(ny);
				Ray r = cam.get_ray(u, v);
				r.normalize_direction();

				//Vector3f p = r.point_at_parameter(2.0);
				c += color(r, world);
			}
			
			c /= float(ns);
			rgb_image[i] = byte(c.x() * 255.99);
			rgb_image[i + 1] = byte(c.y() * 255.99);
			rgb_image[i + 2] = byte(c.z() * 255.99);
			i += RGB_CHANNELS;
		}
	}
	stbi_write_png("ch6.png", nx, ny, RGB_CHANNELS, rgb_image, 0);


	// de-allocation 
	delete[] rgb_image;
	delete world; //owns its list of Hitables and deletes them in its destructor
}
