/**
* Ordering of includes matters.
* stb_image_write.h must be included first, otherwise the
* define _CRT_SECURE_NO_WARNINGS does not go through and VS will
* build with errors saying that fopen may be unsafe.
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

#include <iostream>
#include "ray.h"

typedef uint8_t byte;

/**
* Gives a color that is linerarly interpolated between white and 
* blue-ish based on the direction of the ray.
**/
Vector3f color(const ray &r) {
	Vector3f unit_direction = r.direction().normalized();
	float t = 0.5f*(unit_direction.y() + 1.0f);
	return (1.0f - t)*Vector3f(1.0f, 1.0f, 1.0f) + t * Vector3f(0.5f, 0.7f, 1.0f);
}


int main() {
	constexpr int RGB_CHANNELS = 3;
	int nx = 200;
	int ny = 100;
	byte *rgb_image = new byte[nx*ny*RGB_CHANNELS];

	Vector3f lower_left_corner(-2.0f, -1.0f, -1.0f);
	Vector3f origin(0.0f, 0.0f, 0.0f);

	// Vectors that span the whole image plane
	Vector3f horizontal(4.0f, 0.0f, 0.0f);
	Vector3f vertical(0.0f, 2.0f, 0.0f);

	int i = 0;
	for (int iy = 0; iy < ny; iy++) {
		for (int ix = 0; ix < nx; ix++) {
			float u = float(ix) / float(nx);
			float v = float(ny - iy - 1) / float(ny);
			ray r(origin, lower_left_corner + u*horizontal + v*vertical);
			
			Vector3f c = color(r) * 255.99f; // Offending line
			
			rgb_image[i] = byte(c.x());
			rgb_image[i + 1] = byte(c.y());
			rgb_image[i + 2] = byte(c.z());
			i += RGB_CHANNELS;
		}
	}
	stbi_write_png("ch3.png", nx, ny, RGB_CHANNELS, rgb_image, 0);

	delete[] rgb_image;
}
