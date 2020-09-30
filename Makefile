#-*-Makefile-*-

COMPILER=g++

all: basicc

basicc: src/basicc.cpp
	$(COMPILER) src/basicc.cpp -o bin/basicc

clean:
	rm -f bin/*