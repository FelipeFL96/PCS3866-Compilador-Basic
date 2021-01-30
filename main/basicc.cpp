#include <iostream>
#include <string>
#include <string.h>

#include "lexic.hpp"
#include "syntax.hpp"
#include "semantic.hpp"
#include "generation.hpp"

#include "ASCIIClassifier.hpp"
#include "LexicalAnalyser.hpp"
#include "SyntaxAnalyser.hpp"
#include "SemanticAnalyser.hpp"
#include "CodeGenerator.hpp"

using namespace std;

string type2name(lexic::type t);
string ascii2name(lexic::ascii_type t);

void lex_test(ifstream& file);
void ascii_test(ifstream& file);
void synt_test(ifstream& file);

int main(int argc, char* argv[]) {
    std::cout << "Bem-Vindo ao compilador basicc!" << std::endl;

    if (argc < 2) {
        cerr << "Uso esperado: basicc <entrada.bas> [ <saída.s> ]" << endl;
        return 1;
    }

    string input_file = argv[1];
    string output_file = "out.s";

    ifstream input(input_file);
    if (!input.is_open()) {
        cerr << "\033[1;31mErro: \033[0m" << "Não foi possível abrir o arquivo '" << input_file << "' para entrada" << endl;
        exit(EXIT_FAILURE);
    }

    try {
        if (argc > 2 && 0 == strcmp(argv[2], "-A")) {
            ascii_test(input);
        }
        else if (argc > 2 && 0 == strcmp(argv[2], "-L")) {
            lex_test(input);
        }
        else if (argc > 2 && 0 == strcmp(argv[2], "-S")) {
            //synt_test(file);
        }
        else {
            if (argc > 2)
                output_file = argv[2];

            semantic::SymbolTable symb_table;

            generation::CodeGenerator gen(input_file, output_file, symb_table);
            semantic::SemanticAnalyser smt(input, symb_table, gen);

            smt.run();
        }
    }
    catch (lexic::lexical_exception& e) {
        cerr << "\033[1;31mErro léxico: \033[37;1m" << input_file << "\033[0m" << e.message() << endl;
    }
    catch (syntax::syntax_exception& e) {
        cerr << "\033[1;31mErro sintático: \033[37;1m" << input_file << "\033[0m" << e.message() << endl;
    }
    catch (semantic::semantic_exception& e) {
        cerr << "\033[1;31mErro semântico: \033[37;1m" << input_file << "\033[0m" << e.message() << endl;
    }
    catch (generation::generation_exception& e) {
        cerr << "\033[1;31mErro de geração: \033[0m" << e.message() << endl;
    }

    input.close();

    return 0;
}

void ascii_test(ifstream& file) {
    using namespace lexic;
    
    ASCIIClassifier ac(file);

    while (!file.eof()) {
        ascii_character c = ac.get_next();
        cout << c.pos.position_str() << "\t" << c.character << "\t[" << ascii2name(c.type) << "]" << endl;
    }
}

void lex_test(ifstream& file) {
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