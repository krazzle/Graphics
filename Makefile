###########################################################
# Project 1 Makefile

CC = g++
CFLAGS = -Wall -ggdb
INCLUDE = -I/usr/include
LIBDIR = -L/usr/lib/x86_64-linux-gnu
# Libraries that use native graphics hardware --
LIBS = -lglut -lGLU -lGL -lpthread -lm

###########################################################
# Options if compiling on Mac
UNAME := $(shell uname)
ifeq ($(UNAME), Darwin)
CC = g++
CFLAGS = -Wall -g -D__MAC__
INCLUDE = 
LIBDIR = -L/lusr/X11/lib
LIBS = -framework OpenGL -framework GLUT
endif

###########################################################
# Uncomment the following line if you are using Mesa
#LIBS = -lglut -lMesaGLU -lMesaGL -lm

plant: drawplant.cpp drawplant.h plant.cpp readppm.cpp helper.cpp helper.h l-system-operators.cpp l-system-operators.h
	${CC} ${CFLAGS} ${INCLUDE} -o plant ${LIBDIR} drawplant.cpp plant.cpp readppm.cpp helper.cpp helper.h l-system-operators.cpp l-system-operators.h ${LIBS}

clean:
	rm -f plant *.o core
