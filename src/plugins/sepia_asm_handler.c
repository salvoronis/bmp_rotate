#include <stdio.h>
#include <stdlib.h>
#include "../image.h"

void sepia_sse(struct pixel * img, uint32_t size);

struct image sepia_asm(struct image * img, char * params){
	sepia_sse(img->pixels, img->width * img->height);

	return *img;
}
