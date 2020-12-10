CC = gcc
OUTPUT = build/main
SRC = src/
PLUG = plugins/
OBJ = obj/
LIBS = -lm
LFOLD= -L./lib/

all: proj_struct main.o bmp.o librotation.so libstraight.so sepia_c.so sepia_asm.so
	$(CC) -rdynamic -o $(OUTPUT) $(OBJ)main.o $(OBJ)bmp.o $(LIBS) -ldl

main.o: src/main.c
	$(CC) -o $(OBJ)main.o -c $(SRC)main.c

bmp.o: src/bmp.c
	$(CC) -o $(OBJ)bmp.o -c $(SRC)bmp.c

librotation.so: rotation.o
	$(CC) -shared -o lib/librotation.so obj/rotation.o

rotation.o: src/plugins/rotation.c
	$(CC) -o $(OBJ)rotation.o -c $(SRC)$(PLUG)rotation.c $(LIBS)

libstraight.so: straight.o
	$(CC) -shared -o lib/libstraight.so obj/straight.o

straight.o: src/plugins/straight.c
	$(CC) -o $(OBJ)straight.o -c $(SRC)$(PLUG)straight.c

sepia_c.so: sepia_c.o
	$(CC) -shared -o lib/libsepia_c.so obj/sepia_c.o

sepia_c.o: src/plugins/sepia.c
	$(CC) -o $(OBJ)sepia_c.o -c $(SRC)$(PLUG)sepia.c

sepia_asm.so: sepia_asm_handler.o sepia_asm.o
	$(CC) -shared -fPIC -o lib/libsepia_asm.so $(OBJ)sepia_asm.o $(OBJ)sepia_asm_handler.o

sepia_asm_handler.o: src/plugins/sepia_asm_handler.c
	$(CC) -o $(OBJ)sepia_asm_handler.o -c -fPIC $(SRC)$(PLUG)sepia_asm_handler.c

sepia_asm.o: src/plugins/sepia_asm.asm
	nasm -f elf64 $(SRC)$(PLUG)sepia_asm.asm -o $(OBJ)sepia_asm.o

clean:
	rm obj/* build/* || echo "already cleaned"

rotation: all
	build/main -t rotation -p res/anime.bmp -o res/ress.bmp -a 35

straight: all
	build/main -t straight -p res/anime.bmp -o res/result.bmp

proj_struct:
	mkdir build lib obj res || echo "already exists"

test:
	nasm -f elf64 -g src/plugins/sepia_asm.asm -o obj/sepia_asm.o
	gcc -shared -fPIC -g -o lib/libsepia_asm.so obj/sepia_asm.o obj/sepia_asm_handler.o
	gcc -rdynamic -g -o build/main obj/main.o obj/bmp.o -lm -ldl
	gdb build/main
