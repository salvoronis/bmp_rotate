CC = gcc
OUTPUT = build/main
SRC = src/main.c

all: src/main.c
	$(CC) -o $(OUTPUT) $(SRC)
