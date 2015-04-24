# Makefile: a standard Makefile for MinMaxZ.cpp

OBJS = MinMaxZ.o
TARG = MinMaxZ.cpp
CC = g++ -std=c++11

CFLAGS= -Wall

LIBS = -I/usr/include -I/usr/lib -llas -lboost_filesystem -lboost_system
MinMaxZ : $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o MinMaxZ $(LIBS)



clean:
	rm -f MinMaxZ $(OBJS)

