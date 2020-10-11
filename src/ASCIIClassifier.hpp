#ifndef ASCII_CLASSIFIER_HPP
#define ASCII_CLASSIFIER_HPP

#include <fstream>

#define LINE_START 1
#define COLUMN_START 1

enum ascii_type {
    UNKNOWN,
    DIGIT,
    LETTER,
    SPECIAL,
    DELIMITER
};

class position {
    public:
        int line, column;

        position() {};
        position(int line, int column):
            line(line), column(column) {}
};

class ascii_character {
    public:
        char character;
        ascii_type type;
        position pos;
        
        ascii_character(char character, ascii_type type, position pos) :
            type(type), character(character), pos(pos) {}
};

class ASCIIClassifier {
    public:
        ASCIIClassifier(std::ifstream& file);

        ascii_character get_next();
        ascii_character peek_next();

    private:
        ascii_type classify_character(char c);

        std::ifstream& file;
        int line, column;
};

#endif //ASCII_CLASSIFIER_HPP