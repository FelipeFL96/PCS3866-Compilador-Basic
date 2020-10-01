#ifndef ASCII_CLASSIFIER_HPP
#define ASCII_CLASSIFIER_HPP

#include <fstream>
#include <string>

#define LINE_START 1
#define COLUMN_START 1

enum ascii_type {
    UNKNOWN,
    DIGIT,
    LETTER,
    SPECIAL,
    CONTROL,
    DELIMITER
};

class position {
    public:
        int line, column;

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

class ascii_classifier {
    public:
        ascii_classifier(std::ifstream& file);

        ascii_character get_next();

    private:
        ascii_type classify_character(char c);

        std::ifstream& file;
        int line, column;
};

#endif //ASCII_CLASSIFIER_HPP