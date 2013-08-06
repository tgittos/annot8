####################################################################
# Binaries to create
####################################################################

# The binary
EXECUTABLES = annot8r

####################################################################
# Base variables
####################################################################

# OSTYPE is not set by default on OS X
ifndef OSTYPE
	OSTYPE = $(shell uname -s)
endif

# Select compiler
CXX = g++

# Flags passed to the C++ compiler.
CXXFLAGS = -g -Wall -Wextra

# Source code directory
SRC_DIR = src

####################################################################
# App related variables
####################################################################

ifeq "$(OSTYPE)" "Darwin"
	# Link flags for OS X
	IFLAGS = -Isrc `sdl-config --cflags`
	LDFLAGS = `sdl-config --libs` -framework OpenGL -framework GLUT -lGLEW -lSDL_ttf
else
	# Link flags for Linux
	LDFLAGS = -lGL -lGLU -lglut -lGLEW -lSDL -lSDLmain
endif

####################################################################
# Project build targets
####################################################################

.PHONY: all clean shaders fonts
.DEFAULT: all

all: $(EXECUTABLES) shaders fonts

clean :
	rm -Rf bin
	rm *.o

shaders:
	cp demo* bin/.

fonts:
	cp -R fonts bin/.

annot8r: Annot8r.o Main.o
	mkdir -p bin
	$(CXX) Annot8r.o Main.o $(IFLAGS) $(LDFLAGS) $(CXXFLAGS) -o bin/$@

Annot8r.o: src/Annot8r.hpp src/Annot8r.cpp
	$(CXX) $(IFLAGS) $(CXXFLAGS) -c -o $@ src/Annot8r.cpp

Main.o: src/Main.cpp
	$(CXX) $(IFLAGS) $(CXXFLAGS) -c -o $@ src/Main.cpp
