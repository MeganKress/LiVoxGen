# Makefile: a standard Makefile for VoxGen.cpp

OBJS = VGProj.o
TARG = VGProj.cpp
CC = g++ -std=c++11

CFLAGS= -Wall

LIBS = -I/usr/include -I/usr/lib -llas -lboost_filesystem -lboost_system
VGProj : $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o VGProj $(LIBS)



clean:
	rm -f VGProj $(OBJS)

