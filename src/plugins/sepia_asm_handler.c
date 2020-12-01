#include <stdio.h>
#include <stdlib.h>
#include "../image.h"

void sepia_sse(float*, float*); 

struct image sepia_asm(struct image * img, char * params){
	float x[4] = {1.0f, 2.0f, 3.0f, 4.0f};
	float y[4] = {5.0f, 6.0f, 7.0f, 8.0f};

	sepia_sse(x,y);

	printf("%f %f %f %f",x[0],x[1],x[2],x[3]);
	return *img;
}
