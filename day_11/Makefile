CC=g++

CFLAGS=-O2 -std=c++20 -Wall -I ../include

run: main
	./main

input.inc: input
	sed -f input.sed input > input.inc

main: main.cpp input.inc
	$(CC) $(CFLAGS) -o main main.cpp
