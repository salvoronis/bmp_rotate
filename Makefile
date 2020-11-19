CC = gcc
OUTPUT = build/main
SRC = src/
OBJ = obj/
LIBS = -lm
LFOLD= -L./lib/

all: main.o bmp.o librotation.so libstraight.so
	$(CC) -rdynamic -o $(OUTPUT) $(OBJ)main.o $(OBJ)bmp.o $(LIBS) -ldl

main.o: src/main.c
	$(CC) -o $(OBJ)main.o -c $(SRC)main.c

bmp.o: src/bmp.c
	$(CC) -o $(OBJ)bmp.o -c $(SRC)bmp.c

librotation.so: rotation.o
	$(CC) -shared -o lib/librotation.so obj/rotation.o

rotation.o: src/rotation.c
	$(CC) -o $(OBJ)rotation.o -c $(SRC)rotation.c $(LIBS)

libstraight.so: straight.o
	$(CC) -shared -o lib/libstraight.so obj/straight.o

straight.o: src/straight.c
	$(CC) -o $(OBJ)straight.o -c $(SRC)straight.c

clean:
	rm obj/* build/*

rotation: all
	build/main -t rotation -p res/anime.bmp -o res/ress.bmp -a 35

straight: all
	build/main -t straight -p res/anime.bmp -o res/result.bmp
