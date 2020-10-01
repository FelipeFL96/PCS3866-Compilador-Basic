#include <fstream>
#include <string>

#include "ascii_classifier.hpp"

using namespace std;

ascii_classifier::ascii_classifier(ifstream& file): 
file(file), line(LINE_START), column(COLUMN_START) {}

ascii_character ascii_classifier::get_next() {
    char c;
    ascii_type t;

    if (!file.eof()) {
        file.get(c);
        t = classify_character(c);
    }
    else {
        c = EOF;
        t = CONTROL;
    }
    
    position p(line, column);
    ascii_character ac(c, t, p);

    if (c == '\n') {
        line++;
        column = COLUMN_START;
    }
    else {
        column++;
    }

    return ac;
}

ascii_type ascii_classifier::classify_character(char c) {
        if (
            c == '0' || c == '1' || c == '2' || c == '3' || c == '4' ||
            c == '5' || c == '6' || c == '7' || c == '8' || c == '9'
        )
            return DIGIT;
        else if ( 
            c == 'A' || c == 'B' || c == 'C' || c == 'D' || c == 'E' ||
            c == 'F' || c == 'G' || c == 'H' || c == 'I' || c == 'J' ||
            c == 'K' || c == 'L' || c == 'M' || c == 'N' || c == 'O' ||
            c == 'P' || c == 'Q' || c == 'R' || c == 'S' || c == 'T' ||
            c == 'U' || c == 'V' || c == 'W' || c == 'X' || c == 'Y' ||
            c == 'Z' || c == 'a' || c == 'b' || c == 'c' || c == 'd' ||
            c == 'e' || c == 'f' || c == 'g' || c == 'h' || c == 'i' ||
            c == 'j' || c == 'k' || c == 'l' || c == 'm' || c == 'n' ||
            c == 'o' || c == 'p' || c == 'q' || c == 'r' || c == 's' ||
            c == 't' || c == 'u' || c == 'v' || c == 'w' || c == 'x' ||
            c == 'y' || c == 'z'
        )
            return LETTER;
        else if (
            c == '!' || c == '@' || c == '#' || c == '%' || /*  ̈   */
            c == '&' || c == '*' || c == '(' || c == ')' || c == '_' ||
            c == '+' || c == '-' || c == '='   /*  §  */ || c == '{' ||
            c == '[' || c == 'a' || c == '}' || c == ']' || c == 'o' ||
            c == '?' || c == '/'   /*  °  */ || c == '`' || /* ́   */
            c == '^' || c == '~' || c == '<' || c == ',' || c == '>' ||
            c == '.' || c == ':' || c == ';' || c == '|' || c == '\\'||
            c == '"'
        )
            return SPECIAL;
        else if (
            c == '\n' || c == '\t' || c == '\r'
        )
            return CONTROL;
        else if (c == ' ')
            return DELIMITER;
        else
            return UNKNOWN;
}