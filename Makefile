CC=gcc
CXX=g++
RM=rm -f
CPPFLAGS = -g -std=c++11

SCANNER_SRCS = testScanner.cpp testScanner.h token.cpp token.h scanner.cpp scanner.h FSAtable.h tokenID.h Characters.h
PARSER_SRCS = parser.cpp parser.h tree.h tree.cpp node.h stack.cpp stack.h
MAIN_SRC = main.cpp

SCANNER_OBJS = $(subst .cc,.o,$(SCANNER_SRCS))
PARSER_OBJS = $(subst .cc,.o,$(PARSER_SRCS))
MAIN_OBJS = $(subst .cc,.o,$(MAIN_SRCS))

OBJS = main.cpp parser.cpp parser.h testScanner.cpp testScanner.h token.cpp token.h scanner.cpp scanner.h FSAtable.h tokenID.h Characters.h tree.h tree.cpp node.h stack.cpp stack.h


all: statSem

statSem: $(OBJS) 
	$(CXX) $(CPPFLAGS)  -o statSem $(OBJS)

clean:
	$(RM) $(OBJS)
