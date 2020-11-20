#include <stdint.h>
#include <math.h>
#include "image.h"
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

#define PI 3.14151592654

static struct image rotate_angle(struct image * img, double angle);
static uint32_t rotated_height(struct image * origin, double angle);
static uint32_t rotated_width(struct image * origin, double angle);

struct image rotation(struct image *origin, char* _angle){
	char *rest;
	double angle = strtod(_angle, &rest)*PI/180;
	
	return rotate_angle(origin, angle);
}

uint32_t rotated_height(struct image *origin, double angle){
	return round(fabs(cos(angle))*origin->width+fabs(sin(angle))*origin->height);
}

uint32_t rotated_width(struct image *origin, double angle){
	return round(fabs(sin(angle))*origin->width+fabs(cos(angle))*origin->height);
}

struct image rotate_angle(struct image * origin, double angle){
	uint32_t size = rotated_width(origin, angle);
	uint32_t size1 = rotated_height(origin, angle);
	struct image new_img = creat_image(size1, size);
	
	const float _cos = cos(angle);
	const float _sin = sin(angle);

	const double hwd = (double) new_img.width/2;
	const double hhd = (double) new_img.height/2;
	const double hws = (double) origin->width/2;
	const double hhs = (double) origin->height/2;

	for (int32_t x = 0-hwd; x < new_img.width-hwd; x++) {
		for (int32_t y = 0-hhd; y < new_img.height-hhd; y++) {
			int32_t X = round(x*_cos + y*_sin + hws);
			int32_t Y = round(y*_cos - x*_sin + hhs);

			if (Y <= origin->height && Y >= 0 && X <= origin->width && X >= 0) {
				*(new_img.pixels + ((y+(int32_t)hhd)*new_img.width) + x + (int32_t)hwd) = *(origin->pixels + (Y * origin->width)+X);
			}
		}
	}
	return new_img;
} 
