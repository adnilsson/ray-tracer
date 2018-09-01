/********************************************************************
* Ordering of includes matters.
* stb_image_write.h must be included first, otherwise the
* define _CRT_SECURE_NO_WARNINGS does not go through and VS will 
* build with errors saying that fopen may be unsafe.
********************************************************************/

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>


// Snippet courtesy of http://psgraphics.blogspot.com/2015/06/a-small-image-io-library-stbimageh.html

int main() {
	int x, y, n;
	unsigned char *data = stbi_load("foo.png", &x, &y, &n, 0);
	stbi_write_png("foo_out.png", x, y, 3, data, 0);
}