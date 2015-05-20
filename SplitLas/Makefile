# Makefile: a standard Makefile for VoxGen.cpp

OBJS = SplitLas.o
TARG = SplitLas
CC = g++

CFLAGS= -W -Wall -Wextra -pedantic -std=c++11 -std=gnu++11

LIBS = -I/usr/include -I/usr/lib -llas -lboost_filesystem -lboost_system -lboost_thread

all: $(TARG)

$(TARG) : $(TARG).cpp
	$(CC) $(CFLAGS) -o $(TARG) $(TARG).cpp $(LIBS)

clean:
	$(RM) $(TARG)

