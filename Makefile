INCLUDEDIR=include
CC=gcc
CFLAGS=-g -Wall -I$(INCLUDEDIR) -Itests -lSDL2 -lSDL2main
TESTFOLDER=./tests


all: test



init:
	mkdir -p build

unity.o: $(TESTFOLDER)/unity.c $(TESTFOLDER)/unity.h
	$(CC) $(CFLAGS) -c $(TESTFOLDER)/unity.c -o build/unity.o


test: unity.o unit_test.o chip8.o
	$(CC) $(CFLAGS) build/unity.o build/unit_test.o build/chip8.o -o build/test
	build/test

unit_test.o: $(TESTFOLDER)/unit_test.c
	$(CC) $(CFLAGS) -c $(TESTFOLDER)/unit_test.c -o build/unit_test.o

chip8.o: src/chip8.c include/chip8.h
	$(CC) $(CFLAGS) -c src/chip8.c -o build/chip8.o


clean:
	rm -f build/