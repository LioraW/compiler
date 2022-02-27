CC=gcc
CXX=g++
RM=rm -f
CPPFLAGS = -g -std=c++11

SRCS = main.cpp testScanner.cpp testScanner.h token.cpp token.h scanner.cpp scanner.h FSAtable.h tokenID.h Characters.h
OBJS = $(subst .cc,.o,$(SRCS))

all: main

main: $(OBJS)
	$(CXX) $(CPPFLAGS)  -o main $(OBJS)

clean:
	$(RM) $(OBJS)
