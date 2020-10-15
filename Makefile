# This sample should be run after make libHEAAN.a file in lib folder


CC=g++
CFLAGS=-g -Wall -O2 -std=c++11 -pthread -march=native
LIBS=-lntl -lgmp -lm ./lib/libHEAAN.a -L/usr/local/lib
OBJS=main.o homocomp.o HEAANfunc.o
TARGET=Test

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBS)
	
main.o: homocomp.h main.cpp
	$(CC) $(CFLAGS) -c -o $@ main.cpp

homocomp.o: src/HEAAN.h HEAANfunc.h homocomp.h homocomp.cpp
	$(CC) $(CFLAGS) -c -o $@ homocomp.cpp

HEAANfunc.o: src/HEAAN.h HEAANfunc.h HEAANfunc.cpp
	$(CC) $(CFLAGS) -c -o $@ HEAANfunc.cpp

clean:
	rm *.o
	rm -rf Test
