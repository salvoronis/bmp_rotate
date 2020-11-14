#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "bmp.h"
#include "rotation.h"

int main(int argc, char * argv[]){
	FILE *image = fopen(argv[1], "rb");
	struct bmp_header *header = malloc(sizeof(struct bmp_header));
	if (read_header(image, header) != OK){
		exit(1);
	}
	
	fseek(image, header->bOffbits, SEEK_SET);

	struct image origin = parse_image(image, header->biWidth, header->biHeight);
	fclose(image);
	

	double angle_r = 10.0*PI/180;
	struct image test = rotate_angle(&origin, angle_r);
	struct bmp_header angle_header = rotate_header(header, &test);
	FILE *shit = fopen("./res/angle.bmp", "wb");
	load_image(test, &angle_header, shit);

	struct image rotated = rotate_image(origin);
	struct bmp_header rotated_header = rotate_header(header,&rotated);

	FILE *new_img = fopen("./res/result.bmp", "wb");

	load_image(rotated, &rotated_header, new_img);
	fclose(new_img);

	return 0;
}
