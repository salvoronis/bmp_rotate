CC = gcc
OUTPUT = build/main
SRC = src/main.c src/bmp.c
LIBS = -lm

all: src/main.c
	$(CC) -o $(OUTPUT) $(SRC) $(LIBS)
