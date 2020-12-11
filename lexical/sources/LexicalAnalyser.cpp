#include <fstream>
#include <string>

#include "lexical.hpp"
#include "ASCIIClassifier.hpp"

#include "LexicalAnalyser.hpp"

using namespace std;
using namespace lexical;

LexicalAnalyser::LexicalAnalyser(ifstream& file):
    file(file), ac(file), analyser_state(state::NORMAL) {}

token LexicalAnalyser::get_next() {
    token t;

    if (analyser_state == state::COMMENT) {
        t = read_comment();
        t.type = token_type::CMT;
    }
    else {
        t = extract_token();
        t.type = categorize_token(t.value);
    }

    change_analyser_state(t.type);
    return t;
}

void LexicalAnalyser::change_analyser_state(token_type type) {
    switch(type) {
        case token_type::REM:
            analyser_state = state::COMMENT;
            break;
        case token_type::INT:
        case token_type::PNT:
            analyser_state = state::NUMBER;
            break;
        default:
            analyser_state = state::NORMAL;
    }
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

    if (analyser_state == state::NUMBER && c.character == 'E') {
        c = ac.get_next();
        lexeme.add_char(c.character);
        if (ac.peek_next().type == ascii_type::DIGIT || ac.peek_next().character == '+' || ac.peek_next().character == '-') {
            return lexeme;
        }
    }
 
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
            if (c.type == ascii_type::LETTER) {
                if (c.character != 'E')
                    throw lexical_exception(lexeme.pos, "Identificador inválido");
                else
                    break;
            }
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
        else if (c.character ==  '"') {
            do {
                c = ac.get_next();
                lexeme.add_char(c.character);
                if (ac.peek_next().character == EOF)
                    throw lexical_exception(lexeme.pos, "String não terminada. Esperando '\"'");
            } while (c.character != '"');
        }
    }
    else {
        throw lexical_exception(lexeme.pos, "Caractere não reconhecido");
    }

    return lexeme;
}

token LexicalAnalyser::read_comment() {
    token comment;

    ascii_character c = ac.get_next();
    comment.set_position(c.pos);
    while (c.character != 0xA) {
        comment.add_char(c.character);
        c = ac.get_next();
    }
    comment.add_char(' ');
    return comment;
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
    if (value == "SIN")
        return token_type::FNSIN;
    if (value == "COS")
        return token_type::FNCOS;
    if (value == "TAN")
        return token_type::FNTAN;
    if (value == "ATN")
        return token_type::FNATN;
    if (value == "EXP")
        return token_type::FNEXP;
    if (value == "ABS")
        return token_type::FNABS;
    if (value == "LOG")
        return token_type::FNLOG;
    if (value == "SQR")
        return token_type::FNSQR;
    if (value == "INT")
        return token_type::FNINT;
    if (value == "RND")
        return token_type::FNRND;
    if (value.c_str()[0] == '"')
        return token_type::STR;
    if (value == "E" && analyser_state == state::NUMBER)
        return token_type::EXD;
    if (value.c_str()[0] >= '0' && value.c_str()[0] <= '9')
        return token_type::INT;
    if (value != "")
        return token_type::IDN;
    return token_type::EoF;
}