#include <iostream>
#include <string>

#include "syntax.hpp"
#include "lexic.hpp"
#include "LexicalAnalyser.hpp"
#include "SyntacticalAnalyser.hpp"

#include "Debugger.hpp"

/*#define debug(X) std::cout<<X<< std::endl

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


std::string type_name(lexic::type t) {
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
}*/
