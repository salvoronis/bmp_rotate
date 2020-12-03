#ifndef IMAGE_H
#define IMAGE_H
#include <stdint.h>
struct __attribute__((packed)) pixel {
	uint8_t r, g, b;
};
struct __attribute__((packed)) image {
	uint32_t width,height;
	struct pixel *pixels;
};

enum Error{
	OK,
	FILE_ERR,
	TYPE_ERR,
	BITCOUNT_ERR
};
struct image creat_image(uint32_t width, uint32_t height);
#endif
