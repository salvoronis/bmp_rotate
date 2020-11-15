#include <stdint.h>
#include <math.h>
#include "image.h"
#include "rotation.h"

#include <stdio.h>

uint32_t rotated_height(struct image *origin, double angle){
	return round(cos(angle)*origin->width+sin(angle)*origin->height);
}

uint32_t rotated_width(struct image *origin, double angle){
	return round(sin(angle)*origin->width+cos(angle)*origin->height);
}

struct image rotate_angle(struct image * origin, double angle){
	uint32_t size = rotated_width(origin, angle);
	uint32_t size1 = rotated_height(origin, angle);
	size = size > size1 ? size : size1;
	struct image new_img = creat_image(size,size);
	
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
				*(new_img.pixels + ((i+(int32_t)hwd)*new_img.width) + j + (int32_t)hhd) = *(origin->pixels + (J * origin->width)+I);
			}
		}
	}
	return new_img;
} 
