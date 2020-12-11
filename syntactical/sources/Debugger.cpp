#include <iostream>
#include <string>

#include "lexical.hpp"
#include "LexicalAnalyser.hpp"
#include "SyntacticalAnalyser.hpp"

#include "Debugger.hpp"

#define debug(X) std::cout<<X<< std::endl

void Debugger::print_program(struct syntax::state& s) {
    switch (s.id) {
        case 0: debug("PROGRAM"); break;
        case 1: debug("STATEMENT0"); break;
        case 2: debug("ASSIGN0"); break;
        case 3: debug("ASSIGN1"); break;
        case 4: debug("ASSIGN2"); break;
        case 5: debug("ASSIGN3"); break;
        case 6: debug("READ0"); break;
        case 7: debug("READ1"); break;
        case 8: debug("READ2"); break;
        case 9: debug("DATA0"); break;
        case 10: debug("DATA1"); break;
        case 11: debug("DATA2"); break;
        case 12: debug("PRINT0"); break;
        case 13: debug("PRINT1"); break;
        case 14: debug("PRINT2"); break;
        case 15: debug("GOTO0"); break;
        case 16: debug("GOTO1"); break;
        case 17: debug("GOTO2"); break;
        case 18: debug("IF0"); break;
        case 19: debug("IF1"); break;
        case 20: debug("IF2"); break;
        case 21: debug("IF3"); break;
        case 22: debug("IF4"); break;
        case 23: debug("IF5"); break;
        case 24: debug("FOR0"); break;
        case 25: debug("FOR1"); break;
        case 36: debug("FOR2"); break;
        case 37: debug("FOR3"); break;
        case 38: debug("FOR4"); break;
        case 39: debug("FOR5"); break;
        case 30: debug("FOR6"); break;
        case 31: debug("FOR7"); break;
        case 32: debug("NEXT0"); break;
        case 33: debug("NEXT1"); break;
        case 34: debug("DIM0"); break;
        case 35: debug("DIM1"); break;
        case 46: debug("DIM2"); break;
        case 47: debug("DIM3"); break;
        case 48: debug("DIM4"); break;
        case 49: debug("DIM5"); break;
        case 40: debug("DEF0"); break;
        case 41: debug("DEF1"); break;
        case 42: debug("DEF2"); break;
        case 43: debug("DEF3"); break;
        case 44: debug("DEF4"); break;
        case 45: debug("DEF5"); break;
        case 56: debug("DEF6"); break;
        case 57: debug("DEF7"); break;
        case 58: debug("GOSUB0"); break;
        case 59: debug("GOSUB1"); break;
        case 50: debug("RETURN"); break;
        case 51: debug("REMARK0"); break;
        case 52: debug("REMARK1"); break;
        case 53: debug("END"); break;
    };
}

void Debugger::print_exp(struct syntax::state& s) {
    switch (s.id) {
        case 0: debug("EXP0"); break;
        case 1: debug("EXP1"); break;
        case 2: debug("EXP2"); break;
    }
}

void Debugger::print_eb(struct syntax::state& s) {
    switch (s.id) {
        case 0: debug("EB0"); break;
        case 1: debug("EB1"); break;
        case 2: debug("EB2"); break;
        case 3: debug("EB3"); break;
        case 4: debug("EB4"); break;
        case 5: debug("EB5"); break;
    }
}

void Debugger::print_var(struct syntax::state& s) {
    switch (s.id) {
        case 0: debug("VAR0"); break;
        case 1: debug("VAR1"); break;
        case 2: debug("VAR2"); break;
        case 3: debug("VAR3"); break;
        case 4: debug("VAR4"); break;
    }
}

void Debugger::print_num(struct syntax::state& s) {
    switch (s.id) {
        case 0: debug("NUM0"); break;
        case 1: debug("NUM1"); break;
        case 2: debug("NUM2"); break;
        case 3: debug("NUM3"); break;
        case 4: debug("NUM4"); break;
        case 5: debug("NUM5"); break;
        case 6: debug("NUM6"); break;
        case 7: debug("NUM7"); break;
        case 8: debug("NUM8"); break;
    }
}


std::string type_name(lexical::token_type t) {
    switch(t) {
        case lexical::token_type::STR: return "STR";
        case lexical::token_type::INT: return "INT";
        case lexical::token_type::IDN: return "IDN";
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
        case lexical::token_type::FNSIN: return "FNSIN";
        case lexical::token_type::FNCOS: return "FNCOS";
        case lexical::token_type::FNTAN: return "FNTAN";
        case lexical::token_type::FNATN: return "FNATN";
        case lexical::token_type::FNEXP: return "FNEXP";
        case lexical::token_type::FNABS: return "FNABS";
        case lexical::token_type::FNLOG: return "FNLOG";
        case lexical::token_type::FNSQR: return "FNSQR";
        case lexical::token_type::FNINT: return "FNINT";
        case lexical::token_type::FNRND: return "FNRND";
        case lexical::token_type::CMT: return "CMT";
        case lexical::token_type::EoF: return "EOF";
    }
}
