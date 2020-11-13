CC = gcc
OUTPUT = build/main
SRC = src/main.c src/bmp.c

all: src/main.c
	$(CC) -o $(OUTPUT) $(SRC)
