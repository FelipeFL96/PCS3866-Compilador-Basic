#-*-Makefile-*-

COMPILER=g++

all: basicc

basicc: src/basicc.cpp ASCIIClassifier.o LexicalAnalyser.o
	$(COMPILER) -std=c++11 src/basicc.cpp bin/ASCIIClassifier.o bin/LexicalAnalyser.o -o bin/basicc

LexicalAnalyser.o: src/LexicalAnalyser.hpp src/LexicalAnalyser.cpp
	$(COMPILER) -c -std=c++11 src/LexicalAnalyser.cpp -o bin/LexicalAnalyser.o

ASCIIClassifier.o: src/ASCIIClassifier.hpp src/ASCIIClassifier.cpp
	$(COMPILER) -c -std=c++11 src/ASCIIClassifier.cpp -o bin/ASCIIClassifier.o

clean:
	rm -f bin/*