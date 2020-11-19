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

/*struct bmp_header rotate_header(struct bmp_header *origin, struct image *img){
	struct bmp_header result = *origin;
	result.biHeight = img->height;
	result.biWidth = img->width;
	return result;
}*/

struct bmp_header new_header(struct image origin){
	struct bmp_header new_head;
	new_head.bfType = 0x4D42;
	new_head.biBitCount = 24;
	new_head.biWidth = origin.width;
	new_head.biHeight = origin.height;
	uint8_t padding = (4 - (new_head.biWidth*sizeof(struct pixel))%4)%4;
	new_head.bfileSize = sizeof(struct bmp_header) + (new_head.biWidth*sizeof(struct pixel) + padding)*new_head.biHeight;
	new_head.bfReserved = 0;
	new_head.bOffbits = sizeof(struct bmp_header);
	new_head.biSize = 40;
	new_head.biPlanes = 1;
	new_head.biCompression = 0;
	new_head.biSizeImage = (new_head.biWidth*sizeof(struct pixel)+padding)*new_head.biHeight;
	new_head.biXPelsPerMeter = 0;
	new_head.biYPelsPerMeter = 0;
	new_head.biClrUsed = 0;
	new_head.biClrImportant = 0;
	return new_head;
}

struct image parse_image(FILE *image,uint32_t width, uint32_t height){
	struct image img = creat_image(width, height);
	uint8_t padding = (4 - (img.width * sizeof(struct pixel)) % 4) % 4;
	int32_t pass = 0;
	for (int32_t count = img.height-1; count >= 0; count--) {
		fread(img.pixels+(pass*img.width), sizeof(struct pixel), img.width, image);
		fseek(image, padding, SEEK_CUR);
		pass++;
	}
	return img;
}
