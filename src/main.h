#ifndef MAIN_H
#define MAIN_H
#include <stdint.h>
#include <stdio.h>
struct __attribute__((packed)) bmp_header {
	uint16_t bfType;
	uint32_t bfileSize;
	uint32_t bfReserved;
	uint32_t bOffbits;
	uint32_t biSize;
	uint32_t biWidth;
	uint32_t biHeight;
	uint16_t biPlanes;
	uint16_t biBitCount;
	uint32_t biCompression;
	uint32_t biSizeImage;
	uint32_t biXPelsPerMeter;
	uint32_t biYPelsPerMeter;
	uint32_t biClrUsed;
	uint32_t biClrImportant;
};

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

struct image *parse_image(FILE *image,uint32_t width, uint32_t height);
enum Error read_header(FILE *image, struct bmp_header *header);
struct image *rotate_image(struct image *image);
struct bmp_header rotate_header(struct bmp_header header);
void load_image(struct image *img, struct bmp_header *header, FILE *image);
#endif
