#include <iostream>
#include <string>
#include <string.h>

#include "lexic.hpp"
#include "syntax.hpp"
#include "semantic.hpp"
#include "ASCIIClassifier.hpp"
#include "LexicalAnalyser.hpp"
#include "SyntaxAnalyser.hpp"
#include "SemanticAnalyser.hpp"
#include "CodeGenerator.hpp"

using namespace std;

string type2name(lexic::type t);
string ascii2name(lexic::ascii_type t);

void lex_test(ifstream& file, const char* filename);
void ascii_test(ifstream& file);
void synt_test(ifstream& file);
void sem_test(ifstream& input, ofstream& output);
void gen_test(ifstream& input, ofstream& output);

int main(int argc, char* argv[]) {
    std::cout << "Bem-Vindo ao compilador basicc!" << std::endl;

    if (argc < 2) {
        cerr << "Uso esperado: basicc arquivo.txt" << endl;
        return 1;
    }

    char* filename = argv[1];
    ifstream input(filename);
    ofstream output("out.s");

    try {
        if (argc > 2 && 0 == strcmp(argv[2], "-A")) {
            ascii_test(input);
        }
        else if (argc > 2 && 0 == strcmp(argv[2], "-L")) {
            lex_test(input, filename);
        }
        else if (argc > 2 && 0 == strcmp(argv[2], "-S")) {
            gen_test(input, output);
            //synt_test(file);
        }
        else {
            sem_test(input, output);
        }
    }
    catch (lexic::lexical_exception& e) {
        cerr << "\033[1;31mErro léxico: \033[37;1m" << filename << "\033[0m" << e.message() << endl;
    }
    catch (syntax::syntax_exception& e) {
        cerr << "\033[1;31mErro sintático: \033[37;1m" << filename << "\033[0m" << e.message() << endl;
    }
    catch (semantic::semantic_exception& e) {
        cerr << "\033[1;31mErro semântico: \033[37;1m" << filename << "\033[0m" << e.message() << endl;
    }

    input.close();
    output.close();

    return 0;
}

void ascii_test(ifstream& file) {
    using namespace lexic;
    
    ASCIIClassifier ac(file);
    while (!file.eof()) {
        ascii_character c = ac.get_next();
        cout << c.pos.position_str() << " " << c.character << "\t[" << ascii2name(c.type) << "]" << endl;
    }
}

void lex_test(ifstream& file, const char* filename) {
    using namespace lexic;

    LexicalAnalyser lex(file);
    while (true) {
        token s = lex.get_next();
        if (s.value == "") break;
        cout << "(" << s.pos.line << "," << s.pos.column << ")\t[" << type2name(s.type) << "] " << s.value << endl;
    }
}

void synt_test(ifstream& file) {
    using namespace std;
    using namespace syntax;

    SyntaxAnalyser synt(file);


    //syntax::BStatement a = synt.get_next();

    //cout << "Na linha " << a.get_index() << " li a atribuição de " << a.get_value() << " à variável " << a.get_identifier() << endl;

    //generate_code(a);
}

void sem_test(ifstream& input, ofstream& output) {
    semantic::SemanticAnalyser smt(input, output);
    smt.get_next();
}

void gen_test(ifstream& input, ofstream& output) {
    using namespace std;
    using namespace syntax;

    SyntaxAnalyser stx(input);
    //CodeGenerator gen(output);
    semantic::SemanticAnalyser smt(input, output);

    /*vector<syntax::Elem*> exp = smt.parse_expression();

    gen.generate_header();
    gen.generate_expression(exp);*/


    //syntax::Syntaxeme* sx;
    /*for (int i = 0; i < 1; i++) {
        sx = stx.get_next();
        sx->accept(gen);
        delete sx;
    }*/
    //stx.get_next();

    //gen.generate_variables();

    /*try {
        while (true) {
            token s = lex.get_next();
            if (s.value == "") break;
            cout << "(" << s.pos.line << "," << s.pos.column << ")\t[" << type2name(s.type) << "] " << s.value << endl;
        }
    }
    catch (lexical_exception& e) {
        cerr << "\033[1;31mErro léxico: \033[37;1m" << filename << "\033[0m" << e.message() << endl;
    }*/
}

string ascii2name(lexic::ascii_type t) {
    switch (t) {
        case lexic::ascii_type::UNKNOWN: return "UNKNOWN";
        case lexic::ascii_type::DIGIT: return "DIGIT";
        case lexic::ascii_type::LETTER: return "LETTER";
        case lexic::ascii_type::SPECIAL: return "SPECIAL";
        case lexic::ascii_type::DELIMITER: return "DELIMITER";
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