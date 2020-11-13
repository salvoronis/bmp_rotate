#include "bmp.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "image.h"


void load_image(struct image const img, struct bmp_header * const header, FILE * const image){
	fwrite(header, sizeof(struct bmp_header), 1, image);
	uint8_t padding = (4 - (img.width * sizeof(struct pixel)) % 4) % 4;
	for (uint32_t count = 0; count < img.height; count++) {
		fwrite(img.pixels+(count*img.width), sizeof(struct pixel), img.width, image);
		fwrite(&padding, 1, padding, image);
	}
}

struct image creat_image(uint32_t width, uint32_t height){
	struct image img;
	img.height = height;
	img.width = width;
	img.pixels = (struct pixel*)malloc(img.height*sizeof(struct pixel)*img.width);
	return img;
}

enum Error read_header(FILE *image, struct bmp_header *header){
	if (image == NULL){
		return FILE_ERR;
	}

	fread(header, sizeof(struct bmp_header), 1, image);
	
	if (header->bfType != MAGIC) {
		return TYPE_ERR;
	} else if (header->biBitCount != 24) {
		return BITCOUNT_ERR;
	}
	return OK;
}

struct image rotate_image(struct image origin){
	struct image new_image = creat_image(origin.height, origin.width);

	for (uint32_t i = 0; i<new_image.height; i++) {
		for (uint32_t j = 0; j<new_image.width; j++) {
			*(new_image.pixels + i*new_image.width + j) = *(origin.pixels + j*origin.width + (origin.width-1-i));
		}
	}
	return new_image;
}

struct bmp_header rotate_header(struct bmp_header *origin){
	struct bmp_header result = *origin;
	uint32_t tmp = result.biHeight;
	result.biHeight = result.biWidth;
	result.biWidth = tmp;
	return result;
}

struct image parse_image(FILE *image,uint32_t width, uint32_t height){
	struct image img = creat_image(width, height);
	uint8_t padding = (4 - (img.width * sizeof(struct pixel)) % 4) % 4;
	for (uint32_t count = 0; count < img.height; count++) {
		fread(img.pixels+(count*img.width), sizeof(struct pixel), img.width, image);
		fseek(image, padding, SEEK_CUR);
	}
	return img;
}
