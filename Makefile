CC=gcc
CXX=g++
RM=rm -f
CPPFLAGS = -g -std=c++11

OBJS = main.cpp parser.cpp parser.h testScanner.cpp testScanner.h token.cpp token.h scanner.cpp scanner.h FSAtable.h tokenID.h Characters.h tree.h tree.cpp node.h stack.cpp stack.h codeGen.cpp codeGen.h labels.h

all: statSem

statSem: $(OBJS) 
	$(CXX) $(CPPFLAGS)  -o statSem $(OBJS)

clean:
	$(RM) $(OBJS)
