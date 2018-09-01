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

#include <iostream>
#include <Eigen/Dense>

typedef uint8_t byte;

int main() {
	constexpr int RGB_CHANNELS = 3;
	int nx = 200;
	int ny = 100;
	byte *rgb_image = new byte[nx*ny*RGB_CHANNELS];

	int i = 0;
	for (int iy = 0; iy < ny; iy++) {
		for (int ix = 0; ix < nx; ix++) {
			Eigen::Vector3f color(float(ix) / float(nx), float(ny - iy - 1) / float(ny), 0.2f);
			color *= 255.99f;

			rgb_image[i] = byte(color.x());
			rgb_image[i+1] = byte(color.y());
			rgb_image[i+2] = byte(color.z());
			i += RGB_CHANNELS;
		}
	}
	stbi_write_png("hello_world.png", nx, ny, RGB_CHANNELS, rgb_image, 0);

	delete[] rgb_image;
}