CC=gcc
CXX=g++
RM=rm -f
CPPFLAGS = -g -std=c++11

SRCS = main.cpp
OBJS = $(subst .cc,.o,$(SRCS))

all: main

main: $(OBJS)
	$(CXX) $(CPPFLAGS) $(LDFLAGS) -o main $(OBJS) $(LDLIBS)

clean:
	$(RM) $(OBJS)
