CC=g++

CFLAGS=-O2 -std=c++20 -Wall -I ../include

run: main
	./main

input.o: input.cpp
	$(CC) $(CFLAGS) -o input.o -c input.cpp

main: main.cpp input.o
	$(CC) $(CFLAGS) -o main input.o main.cpp
