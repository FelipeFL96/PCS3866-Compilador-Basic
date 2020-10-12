#include <fstream>
#include <string>

#include "lexical.hpp"
#include "ASCIIClassifier.hpp"

#include "LexicalAnalyser.hpp"

using namespace std;
using namespace lexical;

LexicalAnalyser::LexicalAnalyser(ifstream& file):
    file(file), ac(file) {}

token LexicalAnalyser::get_next() {
    token t = extract_token();
    t.type = categorize_token(t.value);
    return t;
}

token LexicalAnalyser::extract_token() {
    token lexeme;

    while (ac.peek_next().type == ascii_type::DELIMITER) {
        ascii_character c = ac.get_next();
        if (c.character == EOF)
            return lexeme;
    }

    ascii_character c = ac.peek_next();
    lexeme.set_position(c.pos);

    if (c.type == ascii_type::LETTER) {
        while (c.type == ascii_type::LETTER || c.type == ascii_type::DIGIT) {
            lexeme.add_char(ac.get_next().character);
            c = ac.peek_next();
        }
    }
    else if (c.type == ascii_type::DIGIT) {
        while (c.type == ascii_type::DIGIT) {
            lexeme.add_char(ac.get_next().character);
            c = ac.peek_next();
        }
    }
    else if (c.type == ascii_type::SPECIAL) {
        lexeme.add_char(ac.get_next().character);
        if (c.character == '<') {
            c = ac.peek_next();
            if (c.character == '=' || c.character == '>') {
                lexeme.add_char(ac.get_next().character);
            }
        } 
        else if (c.character == '>') {
            c = ac.peek_next();
            if (c.character == '=') {
                lexeme.add_char(ac.get_next().character);
            }
        }
    }
    else {
        throw lexical_exception(lexeme.pos, "Caractere não reconhecido");
    }

    return lexeme;
}

token_type LexicalAnalyser::categorize_token(string& value) {
    if (value == "LET")
        return token_type::LET;
    if (value == "FN")
        return token_type::FN;
    if (value == "DEF")
        return token_type::DEF;
    if (value == "READ")
        return token_type::READ;
    if (value == "DATA")
        return token_type::DATA;
    if (value == "PRINT")
        return token_type::PRINT;
    if (value == "GO")
        return token_type::GO;
    if (value == "TO")
        return token_type::TO;
    if (value == "GOTO")
        return token_type::GOTO;
    if (value == "IF")
        return token_type::IF;
    if (value == "THEN")
        return token_type::THEN;
    if (value == "FOR")
        return token_type::FOR;
    if (value == "STEP")
        return token_type::STEP;
    if (value == "NEXT")
        return token_type::NEXT;
    if (value == "DIM")
        return token_type::DIM;
    if (value == "GOSUB")
        return token_type::GOSUB;
    if (value == "RETURN")
        return token_type::RETURN;
    if (value == "REM")
        return token_type::REM;
    if (value == "END")
        return token_type::END;
    if (value == "+")
        return token_type::ADD;
    if (value == "-")
        return token_type::SUB;
    if (value == "*")
        return token_type::MUL;
    if (value == "/")
        return token_type::DIV;
    if (value == "^")
        return token_type::POW;
    if (value == "=")
        return token_type::EQL;
    if (value == "<>")
        return token_type::NEQ;
    if (value == "<")
        return token_type::LTN;
    if (value == ">")
        return token_type::GTN;
    if (value == "<=")
        return token_type::LEQ;
    if (value == ">=")
        return token_type::GEQ;
    if (value == ",")
        return token_type::COM;
    if (value == ".")
        return token_type::PNT;
    if (value == "(")
        return token_type::PRO;
    if (value == ")")
        return token_type::PRC;
    if (value == "\"")
        return token_type::DQT;
    if (value == "EXD")
        return token_type::EXD;
    if (value == "SIN")
        return token_type::SIN;
    if (value == "COS")
        return token_type::COS;
    if (value == "TAN")
        return token_type::TAN;
    if (value == "ATN")
        return token_type::ATN;
    if (value == "EXP")
        return token_type::EXP;
    if (value == "ABS")
        return token_type::ABS;
    if (value == "LOG")
        return token_type::LOG;
    if (value == "SQR")
        return token_type::SQR;
    if (value == "INT")
        return token_type::INT;
    if (value == "RND")
        return token_type::RND;
    if (value.c_str()[0] >= '0' && value.c_str()[0] <= '9')
        return token_type::INTEGER;
    return token_type::IDENTIFIER;
}