#-*-Makefile-*-

COMPILER=g++

all: basicc

basicc: src/basicc.cpp ASCIIClassifier.o LexicalAnalyser.o
	$(COMPILER) src/basicc.cpp bin/ASCIIClassifier.o bin/LexicalAnalyser.o -o bin/basicc

LexicalAnalyser.o: src/LexicalAnalyser.hpp src/LexicalAnalyser.cpp
	$(COMPILER) -c src/LexicalAnalyser.cpp -o bin/LexicalAnalyser.o

ASCIIClassifier.o: src/ASCIIClassifier.hpp src/ASCIIClassifier.cpp
	$(COMPILER) -c src/ASCIIClassifier.cpp -o bin/ASCIIClassifier.o

clean:
	rm -f bin/*