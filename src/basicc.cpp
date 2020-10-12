#include <iostream>
#include <string>

#include "lexical.hpp"
#include "ASCIIClassifier.hpp"
#include "LexicalAnalyser.hpp"

using namespace std;

string type2name(token_type t);

int main(int argc, char* argv[]) {
    std::cout << "Bem-Vindo ao compilador basicc!" << std::endl;

    if (argc < 2) {
        cerr << "Uso esperado: basicc arquivo.txt" << endl;
        return 1;
    }

    char* filename = argv[1];
    std::ifstream file(filename);

    LexicalAnalyser lex(file);
    try {
        while (true) {
            token s = lex.get_next();
            if (s.value == "") break;
            cout << "(" << s.pos.line << "," << s.pos.column << ")\t[" << type2name(s.type) << "] " << s.value << endl;
        }
    }
    catch (lexical_exception& e) {
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

string type2name(token_type t) {
    switch(t) {
        case IDENTIFIER: return "IDENTIFIER";
        case INTEGER: return "INTEGER";
        case LET: return "LET";
        case FN: return "FN";
        case DEF: return "DEF";
        case READ: return "READ";
        case DATA: return "DATA";
        case PRINT: return "PRINT";
        case GO: return "GO";
        case TO: return "TO";
        case GOTO: return "GOTO";
        case IF: return "IF";
        case THEN: return "THEN";
        case FOR: return "FOR";
        case STEP: return "STEP";
        case NEXT: return "NEXT";
        case DIM: return "DIM";
        case GOSUB: return "GOSUB";
        case RETURN: return "RETURN";
        case REM: return "REM";
        case END: return "END";
        case ADD: return "ADD";
        case SUB: return "SUB";
        case MUL: return "MUL";
        case DIV: return "DIV";
        case POW: return "POW";
        case EQL: return "EQL";
        case NEQ: return "NEQ";
        case LTN: return "LTN";
        case GTN: return "GTN";
        case LEQ: return "LEQ";
        case GEQ: return "GEQ";
        case COM: return "COM";
        case PNT: return "PNT";
        case PRO: return "PRO";
        case PRC: return "PRC";
        case DQT: return "DQT";
        case EXD: return "EXD";
        case SIN: return "SIN";
        case COS: return "COS";
        case TAN: return "TAN";
        case ATN: return "ATN";
        case EXP: return "EXP";
        case ABS: return "ABS";
        case LOG: return "LOG";
        case SQR: return "SQR";
        case INT: return "INT";
        case RND: return "RND";
    }
}