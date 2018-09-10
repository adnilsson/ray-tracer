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
	constexpr int RGB_CHANNELS = 3;
	int nx = 200;
	int ny = 100;
	byte *rgb_image = new byte[nx*ny*RGB_CHANNELS];

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

	int i = 0;
	for (int iy = 0; iy < ny; iy++) {
		for (int ix = 0; ix < nx; ix++) {
			float u = float(ix) / float(nx);
			float v = float(ny - iy - 1) / float(ny);
			Ray r(origin, lower_left_corner + u*horizontal + v*vertical);
			r.normalize_direction();

			Vector3f p = r.point_at_parameter(2.0);
			Vector3f c = color(r, world)*255.99f;
			rgb_image[i] = byte(c.x());
			rgb_image[i + 1] = byte(c.y());
			rgb_image[i + 2] = byte(c.z());
			i += RGB_CHANNELS;
		}
	}
	stbi_write_png("ch5.2.png", nx, ny, RGB_CHANNELS, rgb_image, 0);


	// de-allocation 
	delete[] rgb_image;
	delete world; //ows its list of Hitables and deletes them in its destructor
}
