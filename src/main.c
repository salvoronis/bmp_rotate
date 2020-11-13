#include <stdio.h>
#include <stdlib.h>
#include "bmp.h"

int main(int argc, char * argv[]){
	FILE *image = fopen(argv[1], "rb");
	struct bmp_header *header = malloc(sizeof(struct bmp_header));
	if (read_header(image, header) != OK){
		exit(1);
	}
	
	fseek(image, header->bOffbits, SEEK_SET);

	struct image origin = parse_image(image, header->biWidth, header->biHeight);
	fclose(image);

	struct image rotated = rotate_image(origin);
	struct bmp_header rotated_header = rotate_header(header);

	FILE *new_img = fopen("./res/result.bmp", "wb");

	load_image(rotated, &rotated_header, new_img);
	fclose(new_img);

	return 0;
}
