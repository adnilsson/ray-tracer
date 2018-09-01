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

#include "ray.h"

typedef uint8_t byte;

/**
* Follows the test described by Real Time Rendering 3rd-edition.
* Assumes that Ray r has a normalized direction vector.
* Side-effect: places the most positive t in isect_t if intersection occurs.
* TODO: change to the optimized test.
*/
bool intersects_shpere(const Vector3f& center, float radius, const Ray &r, float &isect_t) {
	Vector3f oc = r.origin() - center;

	float c = oc.dot(oc) - pow(radius, 2);
	float b = r.direction().dot(oc);
	float discriminant = pow(b, 2) - c;

	// Gives true if distcriminant = 0.0f
	if (discriminant + FLT_EPSILON > 0.0f) {
		isect_t = -b - sqrt(discriminant);
		return true;
	}
	return false;
}


/**
* Gives a color that is linerarly interpolated between white and 
* blue-ish based on the direction of the ray.
* Assumes normalized Ray direction vector.
**/
Vector3f color(const Ray &r) {
	Vector3f shpere_center = Vector3f(0, 0, -1);
	float t;
	if (intersects_shpere(shpere_center, 0.5f, r, t)) {
		Vector3f normal = Vector3f(r.point_at_parameter(t) - shpere_center).normalized();
		return 0.5*Vector3f(normal.x()+1, normal.y()+1, normal.z()+1);
	}

	//No intersection. Give background color instead
	t = 0.5f*(r.direction().y() + 1.0f);
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
			Ray r(origin, lower_left_corner + u*horizontal + v*vertical);
			r.normalize_direction();

			Vector3f c = color(r) * 255.99f;
			
			rgb_image[i] = byte(c.x());
			rgb_image[i + 1] = byte(c.y());
			rgb_image[i + 2] = byte(c.z());
			i += RGB_CHANNELS;
		}
	}
	stbi_write_png("ch5.1.png", nx, ny, RGB_CHANNELS, rgb_image, 0);

	delete[] rgb_image;
}
