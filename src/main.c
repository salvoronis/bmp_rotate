#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "bmp.h"
#include "image.h"
#include <dlfcn.h>
#include <getopt.h>
#include <string.h>

char* picture;
char* transform;
char* output;
char* param;

static void disasseembly_arg(int, char**);
static void workspace(struct bmp_header * header, struct image origin);

int main(int argc, char * argv[]){
	disasseembly_arg(argc, argv);

	FILE *image = fopen(picture, "rb");
	struct bmp_header *header = malloc(sizeof(struct bmp_header));
	if (read_header(image, header) != OK){
		exit(1);
	}
	
	fseek(image, header->bOffbits, SEEK_SET);

	struct image origin = parse_image(image, header->biWidth, header->biHeight);
	fclose(image);
	
	workspace(header, origin);

	return 0;
}

void workspace(struct bmp_header *header, struct image origin){
	struct image (*func)(struct image *origin, void* param);
	void *handler;
	char *error;

	char *lib = malloc((strlen(transform)+12)*sizeof(char));
	sprintf(lib, "%s%s%s", "./lib/lib",transform,".so");

	handler = dlopen(lib, RTLD_LAZY);
	if (!handler) {
		fputs(dlerror(), stderr);
		exit(1);
	}
	func = dlsym(handler, transform);
	if ((error = dlerror()) != NULL) {
		fprintf(stderr, "%s\n", error);
		exit(1);
	}

	struct image test = func(&origin, param);
	struct bmp_header angle_header = rotate_header(header, &test);
	FILE *testF = fopen(output, "wb");
	load_image(test, &angle_header, testF);

}

void disasseembly_arg(int argc, char * argv[]){
	const char* shortopts = "t:p:o:a:";
	const struct option longopts[] = {
		{"transform", required_argument, NULL, 't'},
		{"picture", required_argument, NULL, 'p'},
		{"output", required_argument, NULL, 'o'},
		{"arg", required_argument, NULL, 'a'}
	};
	int32_t rez;
	int32_t longind;
	while ((rez = getopt_long(argc, argv, shortopts, longopts, &longind)) != -1){
	switch (rez) {
		case 't':
			if (optarg != NULL) {
				transform = optarg;
			}
		case 'p':
			if (optarg != NULL) {
				picture = optarg;
			}
		case 'o':
			if (optarg != NULL) {
				output = optarg;
			}
		case 'a':
			if (optarg != NULL) {
				param = optarg;
			}
	}
	}

}
