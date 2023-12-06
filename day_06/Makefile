CC=g++

CFLAGS=-O2 -std=c++20 -I ../include

run: main
	./main

input.inc: input input.sed
	sed -f input.sed input > input.inc

main: main.cpp input.inc
	$(CC) $(CFLAGS) -o main main.cpp
