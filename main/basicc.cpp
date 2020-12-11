#include <iostream>
#include <string>
#include <string.h>

#include "lexical.hpp"
#include "ASCIIClassifier.hpp"
#include "LexicalAnalyser.hpp"
#include "SyntacticalAnalyser.hpp"

using namespace std;

string type2name(lexic::type t);
void lex_test(ifstream& file, const char* filename);

int main(int argc, char* argv[]) {
    std::cout << "Bem-Vindo ao compilador basicc!" << std::endl;

    if (argc < 2) {
        cerr << "Uso esperado: basicc arquivo.txt" << endl;
        return 1;
    }

    char* filename = argv[1];
    std::ifstream file(filename);

    if (argc > 2 && 0 == strcmp(argv[2], "L")) {
        try {
        lex_test(file, filename);
        }
        catch (lexic::lexical_exception& e) {
            cerr << "\033[1;31mErro léxico: \033[37;1m" << filename << "\033[0m" << e.message() << endl;
        }
    }
    else {
        try {
        syntax_read(file);
        }
        catch (lexic::lexical_exception& e) {
            cerr << "\033[1;31mErro léxico: \033[37;1m" << filename << "\033[0m" << e.message() << endl;
        }
    }

    file.close();

    return 0;
}

void lex_test(ifstream& file, const char* filename) {
    using namespace lexic;

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
}

string type2name(lexic::type t) {
    switch(t) {
        case lexic::type::STR: return "STR";
        case lexic::type::INT: return "INT";
        case lexic::type::IDN: return "IDN";
        case lexic::type::LET: return "LET";
        case lexic::type::FN: return "FN";
        case lexic::type::DEF: return "DEF";
        case lexic::type::READ: return "READ";
        case lexic::type::DATA: return "DATA";
        case lexic::type::PRINT: return "PRINT";
        case lexic::type::GO: return "GO";
        case lexic::type::TO: return "TO";
        case lexic::type::GOTO: return "GOTO";
        case lexic::type::IF: return "IF";
        case lexic::type::THEN: return "THEN";
        case lexic::type::FOR: return "FOR";
        case lexic::type::STEP: return "STEP";
        case lexic::type::NEXT: return "NEXT";
        case lexic::type::DIM: return "DIM";
        case lexic::type::GOSUB: return "GOSUB";
        case lexic::type::RETURN: return "RETURN";
        case lexic::type::REM: return "REM";
        case lexic::type::END: return "END";
        case lexic::type::ADD: return "ADD";
        case lexic::type::SUB: return "SUB";
        case lexic::type::MUL: return "MUL";
        case lexic::type::DIV: return "DIV";
        case lexic::type::POW: return "POW";
        case lexic::type::EQL: return "EQL";
        case lexic::type::NEQ: return "NEQ";
        case lexic::type::LTN: return "LTN";
        case lexic::type::GTN: return "GTN";
        case lexic::type::LEQ: return "LEQ";
        case lexic::type::GEQ: return "GEQ";
        case lexic::type::COM: return "COM";
        case lexic::type::PNT: return "PNT";
        case lexic::type::PRO: return "PRO";
        case lexic::type::PRC: return "PRC";
        case lexic::type::DQT: return "DQT";
        case lexic::type::EXD: return "EXD";
        case lexic::type::FNSIN: return "FNSIN";
        case lexic::type::FNCOS: return "FNCOS";
        case lexic::type::FNTAN: return "FNTAN";
        case lexic::type::FNATN: return "FNATN";
        case lexic::type::FNEXP: return "FNEXP";
        case lexic::type::FNABS: return "FNABS";
        case lexic::type::FNLOG: return "FNLOG";
        case lexic::type::FNSQR: return "FNSQR";
        case lexic::type::FNINT: return "FNINT";
        case lexic::type::FNRND: return "FNRND";
        case lexic::type::CMT: return "CMT";
        case lexic::type::EoF: return "EOF";
    }
}