INCLUDEDIRS=-Iinclude -I/usr/include/SDL2 -Itests
CC=gcc
CFLAGS=-g -Wall $(INCLUDEDIRS)  
LFLAGS=-lSDL2main -lSDL2
TESTFOLDER=./tests


all: test

init:
	mkdir -p build

unity.o: $(TESTFOLDER)/unity.c $(TESTFOLDER)/unity.h
	$(CC) $(CFLAGS) -c $(TESTFOLDER)/unity.c -o build/unity.o


test: unity.o chip8.o io.o unit_test.o
	$(CC) $(CFLAGS) build/unity.o build/io.o build/unit_test.o build/chip8.o $(LFLAGS) -o build/test
	build/test

unit_test.o: $(TESTFOLDER)/unit_test.c
	$(CC) $(CFLAGS) -c $(TESTFOLDER)/unit_test.c -o build/unit_test.o

chip8.o: src/chip8.c include/chip8.h
	$(CC) $(CFLAGS) -c src/chip8.c -o build/chip8.o

io.o: src/io.c include/io.h
	$(CC) $(CFLAGS) -c src/io.c -o build/io.o

main: chip8.o io.o 
	$(CC) $(CFLAGS) -c src/main.c -o build/main.o
	$(CC) $(CFLAGS) build/main.o build/io.o build/chip8.o $(LFLAGS) -o build/main
	build/main

clean:
	rm -f build/