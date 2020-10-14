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

struct pixel {
	unsigned char b, g, r;
};

enum Error{
	OK,
	FILE_ERR,
	TYPE_ERR,
	BITCOUNT_ERR
};

//void print_header(struct bmp_header*);

int main(void){
	FILE *image = fopen("./res/strelka.bmp", "rb");
	if (image == NULL){
		perror("file error");
		exit(FILE_ERR);
	}
	struct bmp_header *header = malloc(sizeof(struct bmp_header));
	fread(header, sizeof(struct bmp_header), 1, image);
	if (header->bfType != MAGIC) {
		perror("File format error");
		exit(TYPE_ERR);
	} else if (header->biBitCount != 24) {
		perror("Bit count is not 24");
		exit(BITCOUNT_ERR);
	}

	struct pixel rotate[header->biHeight][header->biWidth];

	fseek(image, header->bOffbits, SEEK_SET);

	//print_header(header);
	
	for (int i = header->biHeight-1; i>=0; i--) {
		for (int j = 0; j<header->biWidth; j++) {
			fread(&rotate[i][j],sizeof(struct pixel),1,image);
		}
	}
	fclose(image);
	FILE *rotated = fopen("./res/strelkanew.bmp", "wb");
	fwrite(header, sizeof(struct bmp_header), 1, rotated);
	//print_header(header);
	for (int i = header->biWidth-1; i>=0; i--) {
		for (int j = header->biHeight-1; j>=0; j--) {
			fwrite(&rotate[j][i], sizeof(struct pixel), 1, rotated);
		}
	}

	return 0;
}

/*void print_header(struct bmp_header *header){
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
}*/
