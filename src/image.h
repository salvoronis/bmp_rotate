#ifndef IMAGE_H
#define IMAGE_H
#include <stdint.h>
#define PI 3.14151592654
struct __attribute__((packed)) pixel {
	uint8_t b, g, r;
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
struct image rotate_image(struct image image);
struct image creat_image(uint32_t width, uint32_t height);
struct image calculate_HeWi_angle(struct image *origin, double angle);
struct image rotate_angle(struct image * img, double angle);
#endif
