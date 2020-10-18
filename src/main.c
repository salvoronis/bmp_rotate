#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define MAGIC 0x4d42

struct __attribute__((packed)) bmp_header {
	uint16_t bfType;
	uint32_t bfileSize;
	uint32_t bfReserved;
	uint32_t bOffbits;
	uint32_t biSize;
	uint32_t biWidth;
	uint32_t biHeight;
	uint16_t biPlanes;
	uint16_t biBitCount;
	uint32_t biCompression;
	uint32_t biSizeImage;
	uint32_t biXPelsPerMeter;
	uint32_t biYPelsPerMeter;
	uint32_t biClrUsed;
	uint32_t biClrImportant;
};

struct __attribute__((packed)) pixel {
	unsigned char b, g, r;
};

struct __attribute__((packed)) image {
	uint32_t width,height;
	struct pixel *pixels;
};

enum Error{
	OK,
	FILE_ERR,
	TYPE_ERR,
	BITCOUNT_ERR
};

void print_header(struct bmp_header*);
struct image *parse_image(FILE *image,uint32_t width, uint32_t height);
enum Error read_header(FILE *image, struct bmp_header *header);
struct image *rotate_image(struct image *image);
struct bmp_header rotate_header(struct bmp_header header);
void load_image(struct image *img, struct bmp_header *header, FILE *image);

int main(void){
	FILE *image = fopen("./res/anime.bmp", "rb");
	struct bmp_header *header = malloc(sizeof(struct bmp_header));
	if (read_header(image, header) != OK){
		exit(1);
	}
	
	fseek(image, header->bOffbits, SEEK_SET);

	struct image *origin = parse_image(image, header->biWidth, header->biHeight);

	struct image *rotated = rotate_image(origin);
	struct bmp_header rotated_header = rotate_header(*header);

	FILE *new_img = fopen("./res/newanime.bmp", "wb");
	
	/*fwrite(&rotated_header, sizeof(struct bmp_header), 1, new_img);
	fwrite(rotated->pixels, sizeof(struct pixel), rotated->height*rotated->width, new_img);*/
	load_image(rotated, &rotated_header, new_img);

	return 0;
}

void load_image(struct image *img, struct bmp_header *header, FILE *image){
	fwrite(header, sizeof(struct bmp_header), 1, image);
	fwrite(img->pixels, sizeof(struct pixel), img->height*img->width, image);
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

struct image *rotate_image(struct image *origin){
	struct image *new_image = malloc(sizeof(struct image));
	new_image->height = origin->width;
	new_image->width = origin->height;
	new_image->pixels = (struct pixel*)malloc(new_image->width*sizeof(struct pixel)*new_image->height);

	for (uint32_t i = 0; i<new_image->height; i++) {
		for (uint32_t j = 0; j<new_image->width; j++) {
			*(new_image->pixels + i*new_image->width + j) = *(origin->pixels + j*origin->width + (origin->width-1-i));
		}
	}
	return new_image;
}

struct bmp_header rotate_header(struct bmp_header origin){
	uint32_t tmp = origin.biHeight;
	origin.biHeight = origin.biWidth;
	origin.biWidth = tmp;
	return origin;
}

struct image *parse_image(FILE *image,uint32_t width, uint32_t height){
	struct image *img = malloc(sizeof(struct image));
	//int padding = (4- (width*sizeof(struct pixel)) % 4) % 4;
	img->height = height;
	img->width = width;
	img->pixels = (struct pixel*)malloc(img->width*sizeof(struct pixel)*img->height);

	fread(img->pixels, sizeof(struct pixel), img->height*img->width, image);
	return img;
}

void print_header(struct bmp_header *header){
	printf("type->%X fileSize->%X Reserver->%X offbits->%X Size->%u width->%u height->%u planes->%X bitcount->%u compression->%X sizeimg->%u xpels->%X ypels->%X clrused->%X importatn->%X\n",
			header->bfType,
			header->bfileSize,
			header->bfReserved,
			header->bOffbits,
			header->biSize,
			header->biWidth,
			header->biHeight,
			header->biPlanes,
			header->biBitCount,
			header->biCompression,
			header->biSizeImage,
			header->biXPelsPerMeter,
			header->biYPelsPerMeter,
			header->biClrUsed,
			header->biClrImportant);
}
