#include "bmp.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
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

struct bmp_header rotate_header(struct bmp_header *origin, struct image *img){
	struct bmp_header result = *origin;
	result.biHeight = img->height;
	result.biWidth = img->width;
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

struct image calculate_HeWi_angle(struct image *origin, double angle){
	struct image new_image = *origin;
	new_image.width = round(sin(angle)*origin->width+cos(angle)*origin->height);
	new_image.height = round(cos(angle)*origin->width+sin(angle)*origin->height);
	return new_image;
}


struct image rotate_angle(struct image * origin, double angle){
	struct image tmp = calculate_HeWi_angle(origin, angle);
	struct image new_img = creat_image(tmp.width, tmp.height/*32,32*/);
	const float _cos = cos(angle);
	const float _sin = sin(angle);
	
	const double hwd = (double) new_img.width/2;
	const double hhd = (double) new_img.height/2;
	const double hws = (double) origin->width/2;
	const double hhs = (double) origin->height/2;
	const double r = sqrt(hws*hws + hhs*hhs);
	const double b = atan2(1. * hhs, hws);
	const double cos_b = cos(b);
	const double sin_b = sin(b);

	for (int32_t i = 0-hwd; i < new_img.width-hwd; i++) {
		for (int32_t j = 0-hhd; j < new_img.height-hhd; j++) {
			int32_t I = round(i*_cos - j*_sin + r*cos_b);
			int32_t J = round(i*_sin + j*_cos + r*sin_b);

			if (I < 2*hws && I >= 0 && J < 2*hhs && J >= 0) {
				*(new_img.pixels + ((i+(int32_t)hwd)*new_img.width) + j + (int32_t)hhd) = *(origin->pixels + (I * origin->width)+J);
			}
		}
	}
	return new_img;
} 
