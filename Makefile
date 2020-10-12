COMPILER = g++
FLAGS = -c -std=c++11

OBJPATH = bin

LEXSRC = lexical/sources
LEXHDR = lexical/headers
LEXOBJ = $(OBJPATH)/lexical
MAINSRC = main
MAINHDR = lexical/headers
MAINOBJ = $(OBJPATH)/main

SOURCES = \
	$(wildcard $(LEXSRC)/*.cpp)\
	$(wildcard $(MAINSRC)/*.cpp)
HEADERS = \
	$(wildcard $(LEXHDR)/*.hpp)
OBJECTS = \
	$(subst .cpp,.o,\
	$(subst $(LEXSRC),$(LEXOBJ),\
	$(subst $(MAINSRC),$(MAINOBJ),\
	$(SOURCES))))


all: basicc

basicc: $(OBJECTS)
	$(COMPILER) $^ -o $(OBJPATH)/$@
	cp $(OBJPATH)/$@ ./$@

./$(MAINOBJ)/%.o: ./$(MAINSRC)/%.cpp
	mkdir -p $(MAINOBJ)
	$(COMPILER) $(FLAGS) -I $(MAINHDR) -o $@ $^

./$(LEXOBJ)/%.o: ./$(LEXSRC)/%.cpp ./$(LEXHDR)/%.hpp
	mkdir -p $(LEXOBJ)
	$(COMPILER) $(FLAGS) -I $(LEXHDR) -o $@ $<

clean:
	rm -rf bin/*
	rm -f basicc