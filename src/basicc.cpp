#include <iostream>
#include <string>

#include "lexical.hpp"
#include "ASCIIClassifier.hpp"
#include "LexicalAnalyser.hpp"

using namespace std;

string type2name(lexical::token_type t);

int main(int argc, char* argv[]) {
    std::cout << "Bem-Vindo ao compilador basicc!" << std::endl;

    if (argc < 2) {
        cerr << "Uso esperado: basicc arquivo.txt" << endl;
        return 1;
    }

    char* filename = argv[1];
    std::ifstream file(filename);

    lexical::LexicalAnalyser lex(file);
    try {
        while (true) {
            lexical::token s = lex.get_next();
            if (s.value == "") break;
            cout << "(" << s.pos.line << "," << s.pos.column << ")\t[" << type2name(s.type) << "] " << s.value << endl;
        }
    }
    catch (lexical::lexical_exception& e) {
        cerr << "\033[1;31mErro léxico: \033[37;1m" << filename << "\033[0m" << e.message() << endl;
    }

    /*ASCIIClassifier ac(file);
    cout << ac.get_next().character << endl;*/

    /*while (!file.eof()) {
        token t = lex.get_next();
        cout << "(" << c.pos.line << "," << c.pos.column << ") ";
        printf("[ %.2X  %c ]", (unsigned char) c.character, c.character);
        if (c.type == DIGIT) cout << "DIGIT" << endl;
        if (c.type == LETTER) cout << "LETTER" << endl;
        if (c.type == SPECIAL) cout << "SPECIAL" << endl;
        if (c.type == DELIMITER ) cout << "DELIMITER" << endl;
        if (c.type == UNKNOWN) cout << "UNKNOWN" << endl;
    }*/

    file.close();

    return 0;
}

string type2name(lexical::token_type t) {
    switch(t) {
        case lexical::token_type::IDENTIFIER: return "IDENTIFIER";
        case lexical::token_type::INTEGER: return "INTEGER";
        case lexical::token_type::LET: return "LET";
        case lexical::token_type::FN: return "FN";
        case lexical::token_type::DEF: return "DEF";
        case lexical::token_type::READ: return "READ";
        case lexical::token_type::DATA: return "DATA";
        case lexical::token_type::PRINT: return "PRINT";
        case lexical::token_type::GO: return "GO";
        case lexical::token_type::TO: return "TO";
        case lexical::token_type::GOTO: return "GOTO";
        case lexical::token_type::IF: return "IF";
        case lexical::token_type::THEN: return "THEN";
        case lexical::token_type::FOR: return "FOR";
        case lexical::token_type::STEP: return "STEP";
        case lexical::token_type::NEXT: return "NEXT";
        case lexical::token_type::DIM: return "DIM";
        case lexical::token_type::GOSUB: return "GOSUB";
        case lexical::token_type::RETURN: return "RETURN";
        case lexical::token_type::REM: return "REM";
        case lexical::token_type::END: return "END";
        case lexical::token_type::ADD: return "ADD";
        case lexical::token_type::SUB: return "SUB";
        case lexical::token_type::MUL: return "MUL";
        case lexical::token_type::DIV: return "DIV";
        case lexical::token_type::POW: return "POW";
        case lexical::token_type::EQL: return "EQL";
        case lexical::token_type::NEQ: return "NEQ";
        case lexical::token_type::LTN: return "LTN";
        case lexical::token_type::GTN: return "GTN";
        case lexical::token_type::LEQ: return "LEQ";
        case lexical::token_type::GEQ: return "GEQ";
        case lexical::token_type::COM: return "COM";
        case lexical::token_type::PNT: return "PNT";
        case lexical::token_type::PRO: return "PRO";
        case lexical::token_type::PRC: return "PRC";
        case lexical::token_type::DQT: return "DQT";
        case lexical::token_type::EXD: return "EXD";
        case lexical::token_type::SIN: return "SIN";
        case lexical::token_type::COS: return "COS";
        case lexical::token_type::TAN: return "TAN";
        case lexical::token_type::ATN: return "ATN";
        case lexical::token_type::EXP: return "EXP";
        case lexical::token_type::ABS: return "ABS";
        case lexical::token_type::LOG: return "LOG";
        case lexical::token_type::SQR: return "SQR";
        case lexical::token_type::INT: return "INT";
        case lexical::token_type::RND: return "RND";
    }
}