# Makefile: a standard Makefile for VoxGen.cpp

OBJS = SplitLas.o
TARG = SplitLas.cpp
CC = g++ -std=c++11

CFLAGS= -Wall

LIBS = -I/usr/include -I/usr/lib -llas
SplitLas : $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o SplitLas $(LIBS)



clean:
	rm -f SplitLas $(OBJS)

