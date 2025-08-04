CXXFLAGS=-g

all: testrun

testrun: testrun.o hw2.o store.o
	g++ -o testrun testrun.o hw2.o store.o -lpthread

testrun.o: hw2.h store.h testrun.cpp 

clean:
	rm testrun.o hw2.o store.o testrun
