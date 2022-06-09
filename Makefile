CC=gcc
CXX=g++
RM=rm -f
CPPFLAGS = -g -std=c++11

SRC = main.cpp parser.cpp parser.h token.cpp token.h scanner.cpp scanner.h FSAtable.h tokenID.h Characters.h tree.h tree.cpp node.h stack.cpp stack.h labels.h statSem.cpp statSem.h utils.cpp utils.h

all: compfs

compfs: $(SRC) 
	$(CXX) $(CPPFLAGS)  -o compfs $(SRC)


clean:
	$(RM) compfs
