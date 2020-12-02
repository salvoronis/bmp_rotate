#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "bmp.h"
#include "image.h"
#include <dlfcn.h>
#include <getopt.h>
#include <string.h>
#include <sys/time.h>
#include <sys/resource.h>

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
	struct rusage r;
	struct timeval start;
	struct timeval end;

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

	getrusage(RUSAGE_SELF, &r);
	start = r.ru_utime;
	struct image test = func(&origin, param);
	getrusage(RUSAGE_SELF, &r);
	end = r.ru_utime;

	long timer = ((end.tv_sec - start.tv_sec) * 1000000L) + end.tv_usec - start.tv_usec;
	printf("Time -> %ld\n", timer);

	struct bmp_header head = new_header(test);
	FILE *testF = fopen(output, "wb");
	load_image(test, &head, testF);

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
