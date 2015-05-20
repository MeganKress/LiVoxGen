# Makefile: a standard Makefile for VoxGen.cpp

OBJS = point.o vgpar.o voxcol.o voxdata.o voxel.o VoxGen.o
TARG = point.cpp vgpar.cpp voxcol.cpp voxdata.cpp voxel.cpp VoxGen.cpp
CC = g++ -std=c++11

CFLAGS= -Wall

LIBS = -I/usr/include -I/usr/lib -lgdal -llas -lkdtree -lboost_filesystem -lboost_system -lgdal
VoxGen : $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o VoxGen $(LIBS)



clean:
	rm -f VoxGen $(OBJS)

