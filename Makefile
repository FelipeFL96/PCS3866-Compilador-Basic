#-*-Makefile-*-

COMPILER=g++

all: basicc

basicc: src/basicc.cpp ascii_classifier.o
	$(COMPILER) src/basicc.cpp bin/ascii_classifier.o -o bin/basicc

ascii_classifier.o: src/ascii_classifier.hpp src/ascii_classifier.cpp
	$(COMPILER) -c src/ascii_classifier.cpp -o bin/ascii_classifier.o

clean:
	rm -f bin/*