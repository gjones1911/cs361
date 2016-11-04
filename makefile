#CS 360 Lab 1 Makefile

CC = g++

INCLUDES = -I/home/plank/cs360/include

CFLAGS = -g $(INCLUDES)

LIBDIR = /home/plank/cs360/objs

LIBS = $(LIBDIR)/libfdr.a

EXECUTABLES: jobscheduler 

all: $(EXECUTABLES)

.SUFFIXES: .c .o
.c.o:
	$(CC) $(CFLAGS) -c $*.c

jobscheduler: jobs.cpp
	    $(CC) $(CFLAGS) -o jobscheduler jobs.cpp


#make clean will rid your directory of the executable,
#object files, and any core dumps you've caused
clean:
	rm core $(EXECUTABLES) *.o


