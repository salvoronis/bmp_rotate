#include "image.h"
#include <stdint.h>

struct image straight(struct image * origin, char* nil){
	return rotate_image(*origin);
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
