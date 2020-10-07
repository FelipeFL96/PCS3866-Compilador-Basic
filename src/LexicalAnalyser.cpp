#include <iostream>
#include <fstream>
#include <exception>
#include <string>

#include "ASCIIClassifier.hpp"

#include "LexicalAnalyser.hpp"

using namespace std;

LexicalAnalyser::LexicalAnalyser(ifstream& file):
    file(file), ac(file) {}

token LexicalAnalyser::get_next() {
   token t = extract_token();
   t.type = categorize_token(t.value);
   return t;
}

token LexicalAnalyser::extract_token() {
    token lexeme;
    ascii_character c = ac.get_next();

    while (c.type == DELIMITER) {
        if (c.character == EOF)
            return lexeme;
        c = ac.get_next();
    }

    lexeme.set_position(c.pos);

    if (c.type == LETTER) {
        while (c.type == LETTER || c.type == DIGIT) {
            lexeme.add_char(c.character);
            c = ac.get_next();
        }
        ac.back();
    }
    else if (c.type == DIGIT) {
        while (c.type == DIGIT) {
            lexeme.add_char(c.character);
            c = ac.get_next();
        }
        ac.back();
    }
    else if (c.type == SPECIAL) {
        lexeme.add_char(c.character);
        if (c.character == '<') {
            c = ac.get_next();
            if (c.character == '=' || c.character == '>') {
                lexeme.add_char(c.character);
            }
            else {
                ac.back();
            }
        } 
        else if (c.character == '>') {
            c = ac.get_next();
            if (c.character == '=') {
                lexeme.add_char(c.character);
            }
            else {
                ac.back();
            }
        }
    }
    else {
        cout << "EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEPA!!!!!" << endl;
    }

    return lexeme;
}

token_type LexicalAnalyser::categorize_token(string value) {
    if (value == "LET")
        return LET;
    if (value == "FN")
        return FN;
    if (value == "DEF")
        return DEF;
    if (value == "READ")
        return READ;
    if (value == "DATA")
        return DATA;
    if (value == "PRINT")
        return PRINT;
    if (value == "GO")
        return GO;
    if (value == "TO")
        return TO;
    if (value == "GOTO")
        return GOTO;
    if (value == "IF")
        return IF;
    if (value == "THEN")
        return THEN;
    if (value == "FOR")
        return FOR;
    if (value == "STEP")
        return STEP;
    if (value == "NEXT")
        return NEXT;
    if (value == "DIM")
        return DIM;
    if (value == "GOSUB")
        return GOSUB;
    if (value == "RETURN")
        return RETURN;
    if (value == "REM")
        return REM;
    if (value == "END")
        return END;
    if (value == "+")
        return ADD;
    if (value == "-")
        return SUB;
    if (value == "*")
        return MUL;
    if (value == "/")
        return DIV;
    if (value == "^")
        return POW;
    if (value == "=")
        return EQL;
    if (value == "<>")
        return NEQ;
    if (value == "<")
        return LTN;
    if (value == ">")
        return GTN;
    if (value == "<=")
        return LEQ;
    if (value == ">=")
        return GEQ;
    if (value == ",")
        return COM;
    if (value == ".")
        return PNT;
    if (value == "(")
        return PRO;
    if (value == ")")
        return PRC;
    if (value == "\"")
        return DQT;
    if (value == "EXD")
        return EXD;
    if (value == "SIN")
        return SIN;
    if (value == "COS")
        return COS;
    if (value == "TAN")
        return TAN;
    if (value == "ATN")
        return ATN;
    if (value == "EXP")
        return EXP;
    if (value == "ABS")
        return ABS;
    if (value == "LOG")
        return LOG;
    if (value == "SQR")
        return SQR;
    if (value == "INT")
        return INT;
    if (value == "RND")
        return RND;
}
