#ifndef BMP_H
#define BMP_H
#define MAGIC 0x4d42
#include <stdint.h>
#include <stdio.h>
#include "image.h"
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

struct image parse_image(FILE *image,uint32_t width, uint32_t height);
enum Error read_header(FILE *image, struct bmp_header *header);
struct bmp_header rotate_header(struct bmp_header *header);
void load_image(struct image const img, struct bmp_header * const header, FILE * const image);
#endif
