#include <iostream>
#include <string>

#include "lexical.hpp"
#include "LexicalAnalyser.hpp"

#include "SyntacticalAnalyser.hpp"

using namespace syntax;

lexical::token tk;

const struct state PROGRAM    = { 0, false};
const struct state STATEMENT0 = { 1, false};
const struct state ASSIGN0    = { 2, false};
const struct state ASSIGN1    = { 3, false};
const struct state ASSIGN2    = { 4, false};
const struct state ASSIGN3    = { 5, false};
const struct state READ0      = { 6, false};
const struct state READ1      = { 7, false};
const struct state READ2      = { 8, false};
const struct state DATA0      = { 9, false};
const struct state DATA1      = {10, false};
const struct state DATA2      = {11, false};
const struct state PRINT0     = {12, false};
const struct state PRINT1     = {13, false};
const struct state PRINT2     = {14, false};
const struct state GOTO0      = {15, false};
const struct state GOTO1      = {16, false};
const struct state GOTO2      = {17, false};
const struct state IF0        = {18, false};
const struct state IF1        = {19, false};
const struct state IF2        = {20, false};
const struct state IF3        = {21, false};
const struct state IF4        = {22, false};
const struct state IF5        = {23, false};
const struct state IF6        = {24, false};
const struct state IF7        = {25, false};
const struct state IF8        = {26, false};
const struct state IF9        = {27, false};
const struct state FOR0       = {28, false};
const struct state FOR1       = {29, false};
const struct state FOR2       = {30, false};
const struct state FOR3       = {31, false};
const struct state FOR4       = {32, false};
const struct state FOR5       = {33, false};
const struct state FOR6       = {34, false};
const struct state FOR7       = {35, false};
const struct state NEXT0      = {36, false};
const struct state NEXT1      = {37, false};
const struct state DIM0       = {38, false};
const struct state DIM1       = {39, false};
const struct state DIM2       = {40, false};
const struct state DIM3       = {41, false};
const struct state DIM4       = {42, false};
const struct state DIM5       = {43, false};
const struct state DEF0       = {44, false};
const struct state DEF1       = {45, false};
const struct state DEF2       = {46, false};
const struct state DEF3       = {47, false};
const struct state DEF4       = {48, false};
const struct state DEF5       = {49, false};
const struct state DEF6       = {50, false};
const struct state DEF7       = {51, false};
const struct state GOSUB0     = {52, false};
const struct state GOSUB1     = {53, false};
const struct state RETURN     = {54, false};
const struct state REMARK0    = {55, false};
const struct state REMARK1    = {56, false};
const struct state END        = {57, true};

/*#define PROGRAM_FSM_SIZE 60
const struct transition program[PROGRAM_FSM_SIZE] = {
    {PROGRAM,    true, lexical::token_type::INT, STATEMENT0},
    // Bloco ASSIGN
    {STATEMENT0, true,  lexical::token_type::LET, ASSIGN0},
    {ASSIGN0,    true,  lexical::token_type::IDN, ASSIGN1},
    {ASSIGN1,    true,  lexical::token_type::EQL, ASSIGN2},
    {ASSIGN2,    false, lexical::token_type::EXPRESSION, ASSIGN3},
    {ASSIGN3,    false, lexical::token_type::INT, STATEMENT0},
    // Bloco READ
    {STATEMENT0, true, lexical::token_type::READ, READ0},
    {READ0,      true, lexical::token_type::VAR,  READ1},
    {READ1,      true, lexical::token_type::COM,  READ2},
    {READ1,      true, lexical::token_type::INT,  STATEMENT0},
    {READ2,      true, lexical::token_type::VAR,  READ1},
    // Bloco DATA
    {STATEMENT0, true, lexical::token_type::DATA, DATA0},
    {DATA0,      true, lexical::token_type::SNUM, DATA1},
    {DATA1,      true, lexical::token_type::COM,  DATA2},
    {DATA1,      true, lexical::token_type::INT,  STATEMENT0},
    {DATA2,      true, lexical::token_type::SNUM, DATA1},
    // Bloco PRINT
    {STATEMENT0, true, lexical::token_type::PRINT, PRINT0},
    {PRINT0,     true, lexical::token_type::PITEM, PRINT1},
    {PRINT0,     true, lexical::token_type::INT,   STATEMENT0},
    {PRINT1,     true, lexical::token_type::COM,   PRINT2},
    {PRINT1,     true, lexical::token_type::INT,   STATEMENT0},
    {PRINT2,     true, lexical::token_type::PITEM, PRINT1},
    {PRINT2,     true, lexical::token_type::INT,   STATEMENT0},
    // Bloco GOTO
    {STATEMENT0, true, lexical::token_type::GO,   GOTO0},
    {STATEMENT0, true, lexical::token_type::GOTO, GOTO1},
    {GOTO0,      true, lexical::token_type::TO,   GOTO1},
    {GOTO1,      true, lexical::token_type::INT,  GOTO2},
    {GOTO2,      true, lexical::token_type::INT,  STATEMENT0},
    // Bloco IF
    {STATEMENT0, true, lexical::token_type::IF,   IF0},
    {IF0,        true, lexical::token_type::GEQ,  IF1},
    {IF0,        true, lexical::token_type::GTN,  IF2},
    {IF0,        true, lexical::token_type::NEQ,  IF3},
    {IF0,        true, lexical::token_type::LTN,  IF4},
    {IF0,        true, lexical::token_type::LEQ,  IF5},
    {IF0,        true, lexical::token_type::EQL,  IF6},
    {IF1,        true, lexical::token_type::GEQ,  IF7},
    {IF2,        true, lexical::token_type::GTN,  IF7},
    {IF3,        true, lexical::token_type::NEQ,  IF7},
    {IF4,        true, lexical::token_type::LTN,  IF7},
    {IF5,        true, lexical::token_type::LEQ,  IF7},
    {IF6,        true, lexical::token_type::EQL,  IF7},
    {IF7,        true, lexical::token_type::THEN, IF8},
    {IF8,        true, lexical::token_type::INT,  IF9},
    {IF9,        true, lexical::token_type::INT,  STATEMENT0},
    // Bloco FOR
    {STATEMENT0, true, lexical::token_type::FOR,  FOR0},
    {FOR0,       true, lexical::token_type::IDN,  FOR1},
    {FOR1,       true, lexical::token_type::EQL,  FOR2},
    {FOR2,       true, lexical::token_type::EXPRESSION, FOR3},
    {FOR3,       true, lexical::token_type::TO,   FOR4},
    {FOR4,       true, lexical::token_type::EXPRESSION, FOR5},
    {FOR5,       true, lexical::token_type::STEP, FOR6},
    {FOR5,       true, lexical::token_type::INT,  STATEMENT0},
    {FOR6,       true, lexical::token_type::EXPRESSION, FOR7},
    {FOR7,       true, lexical::token_type::INT,  STATEMENT0},
    // Bloco NEXT
    {STATEMENT0, true, lexical::token_type::NEXT, NEXT0},
    {NEXT0,      true, lexical::token_type::IDN,  NEXT1},
    {NEXT1,      true, lexical::token_type::INT,  STATEMENT0},
    // Bloco DIM
    {STATEMENT0, true, lexical::token_type::DIM, DIM0},
    {DIM0,       true, lexical::token_type::IDN, DIM1},
    {DIM1,       true, lexical::token_type::PRO, DIM2},
    {DIM2,       true, lexical::token_type::INT, DIM3},
    {DIM3,       true, lexical::token_type::PRC, DIM4},
    {DIM4,       true, lexical::token_type::COM, DIM5},
    {DIM4,       true, lexical::token_type::INT, STATEMENT0},
    {DIM5,       true, lexical::token_type::IDN, DIM1},
    // Bloco DEF
    {STATEMENT0, true, lexical::token_type::DEF, DEF0},
    {DEF0,       true, lexical::token_type::FN,  DEF1},
    {DEF1,       true, lexical::token_type::IDN, DEF2},
    {DEF2,       true, lexical::token_type::PRO, DEF3},
    {DEF3,       true, lexical::token_type::IDN, DEF4},
    {DEF4,       true, lexical::token_type::PRC, DEF5},
    {DEF5,       true, lexical::token_type::EQL, DEF6},
    {DEF6,       true, lexical::token_type::EXPRESSION, DEF7},
    {DEF7,       true, lexical::token_type::INT, STATEMENT0},
    // Bloco GOSUB
    {STATEMENT0, true, lexical::token_type::GOSUB, GOSUB0},
    {GOSUB0,     true, lexical::token_type::INT,   GOSUB1},
    {GOSUB1,     true, lexical::token_type::INT,   STATEMENT0},
    // Bloco RETURN
    {STATEMENT0, true, lexical::token_type::RETURN, RETURN},
    {RETURN,     true, lexical::token_type::INT, STATEMENT0},
    // Bloco REM
    {STATEMENT0, true, lexical::token_type::REM, REMARK0},
    {REMARK0,    true, lexical::token_type::STR, REMARK1},
    {REMARK1,    true, lexical::token_type::INT, STATEMENT0},
    // Bloco END
    {STATEMENT0, true, lexical::token_type::END, END},
};*/


#define EXP_FSM_SIZE 9
const struct state EXP0 = {0, false};
const struct state EXP1 = {1, true};
const struct state EXP2 = {2, true};

const struct transition exp[EXP_FSM_SIZE] = {
    {EXP0, true,  (int) lexical::token_type::ADD, EXP0},
    {EXP0, true,  (int) lexical::token_type::SUB, EXP0},
    {EXP0, false, (int) type::EB,  EXP1},
    {EXP1, true,  (int) lexical::token_type::ADD, EXP2},
    {EXP1, true,  (int) lexical::token_type::SUB, EXP2},
    {EXP1, true,  (int) lexical::token_type::MUL, EXP2},
    {EXP1, true,  (int) lexical::token_type::DIV, EXP2},
    {EXP1, true,  (int) lexical::token_type::POW, EXP2},
    {EXP2, false, (int) type::EB,  EXP1},
};

bool FSM_exp(lexical::LexicalAnalyser& lex) {
    struct state s = EXP0;
    std::cout << "<EXP>" << std::endl;
    while (true) {
        std::cout << "Estado " << s.id << ": [" << type_name(tk.type) << "] " << tk.value << std::endl;
        const struct transition *t = NULL;

        for (int i = 0; i < EXP_FSM_SIZE; i++) {
            if (exp[i].current.id == s.id) {
                if (exp[i].input_is_terminal && (lexical::token_type)exp[i].input == tk.type) {
                    t = &exp[i];
                    tk = lex.get_next();
                    break;
                }
                else if ((type)exp[i].input == type::EB && FSM_eb(lex)) {
                    t = &exp[i];
                    break;
                }
            }
        }

        if (t) {
            s = t->next;
            continue;
        }

        if (s.accepting) {
            std::cout << "</EXP> : APROVADO" << std::endl;
            return true;
        }
        else {
            std::cout << "</EXP> : REPROVADO" << std::endl;
            return false;
        }
    }
}

#define EB_FSM_SIZE 18
const struct state EB0 = {0, false};
const struct state EB1 = {1, false};
const struct state EB2 = {2, false};
const struct state EB3 = {3, false};
const struct state EB4 = {4, false};
const struct state EB5 = {5, true};

struct transition eb[EB_FSM_SIZE] = {
    {EB0, true,  (int) lexical::token_type::PRO,   EB1},
    {EB0, true,  (int) lexical::token_type::FNSIN, EB3},
    {EB0, true,  (int) lexical::token_type::FNCOS, EB3},
    {EB0, true,  (int) lexical::token_type::FNTAN, EB3},
    {EB0, true,  (int) lexical::token_type::FNATN, EB3},
    {EB0, true,  (int) lexical::token_type::FNEXP, EB3},
    {EB0, true,  (int) lexical::token_type::FNABS, EB3},
    {EB0, true,  (int) lexical::token_type::FNLOG, EB3},
    {EB0, true,  (int) lexical::token_type::FNSQR, EB3},
    {EB0, true,  (int) lexical::token_type::FNINT, EB3},
    {EB0, true,  (int) lexical::token_type::FNRND, EB3},
    {EB0, true,  (int) lexical::token_type::FN,    EB4},
    {EB0, false, (int) type::VAR,                  EB5},
    {EB0, false, (int) type::NUM,                  EB5},
    {EB1, true,  (int) type::EXP,                  EB2},
    {EB2, true,  (int) lexical::token_type::PRC,   EB5},
    {EB3, true,  (int) lexical::token_type::PRO,   EB1},
    {EB4, true,  (int) lexical::token_type::IDN,   EB3}    
};

bool FSM_eb(lexical::LexicalAnalyser& lex) {
    struct state s = EB0;
    std::cout << "<EB>" << std::endl;
    while (true) {
        std::cout << "Estado " << s.id << ": [" << type_name(tk.type) << "] " << tk.value << std::endl;
        const struct transition *t = NULL;

        for (int i = 0; i < EB_FSM_SIZE; i++) {
            if (eb[i].current.id == s.id) {
                if (eb[i].input_is_terminal && (lexical::token_type)eb[i].input == tk.type) {
                    t = &eb[i];
                    tk = lex.get_next();
                    break;
                }
                else if ((type)eb[i].input == type::NUM && FSM_num(lex)) {
                    t = &eb[i];
                    break;
                }
                else if ((type)eb[i].input == type::VAR && FSM_var(lex)) {
                    t = &eb[i];
                    break;
                }
                else if ((type)eb[i].input == type::EXP && FSM_exp(lex)) {
                    t = &eb[i];
                    break;
                }
            }
        }

        if (t) {
            s = t->next;
            continue;
        }

        if (s.accepting) {
            std::cout << "</EB> : APROVADO" << std::endl;
            return true;
        }
        else {
            std::cout << "</EB> : REPROVADO" << std::endl;
            return false;
        }
    }
}


#define VAR_FSM_SIZE 15
const struct state VAR0 = {0, false};
const struct state VAR1 = {1, true};
const struct state VAR2 = {2, false};
const struct state VAR3 = {3, false};
const struct state VAR4 = {4, true};

const struct transition var[5] = {
    {VAR0, true,  (int) lexical::token_type::IDN, VAR1},
    {VAR1, true,  (int) lexical::token_type::PRO, VAR2},
    {VAR2, false, (int) type::EXP,                VAR3},
    {VAR3, true,  (int) lexical::token_type::COM, VAR2},
    {VAR3, true,  (int) lexical::token_type::PRC, VAR4},
};

bool FSM_var(lexical::LexicalAnalyser& lex) {
    struct state s = VAR0;
    std::cout << "<VAR>" << std::endl;
    while (true) {
        std::cout << "Estado " << s.id << ": [" << type_name(tk.type) << "] " << tk.value << std::endl;
        const struct transition *t = NULL;

        for (int i = 0; i < VAR_FSM_SIZE; i++) {
            if (var[i].current.id == s.id) {
                if ((lexical::token_type)var[i].input == tk.type) {
                    t = &var[i];
                    tk = lex.get_next();
                    break;
                }
                else if ((type)var[i].input == type::EXP && FSM_exp(lex)) {
                    t = &var[i];
                    break;
                }
            }
        }

        if (t) {
            s = t->next;
            continue;
        }

        if (s.accepting) {
            std::cout << "</VAR> : APROVADO" << std::endl;
            return true;
        }
        else {
            std::cout << "</VAR> : REPROVADO" << std::endl;
            return false;
        }
    }
}

#define NUM_FSM_SIZE 15
const struct state NUM0 = {0, false};
const struct state NUM1 = {1, false};
const struct state NUM2 = {2, false};
const struct state NUM3 = {3, true};
const struct state NUM4 = {4, false};
const struct state NUM5 = {5, true};
const struct state NUM6 = {6, false};
const struct state NUM7 = {7, false};
const struct state NUM8 = {8, true};

const struct transition num[NUM_FSM_SIZE] = {
    {NUM0, true, (int) lexical::token_type::ADD, NUM1},
    {NUM0, true, (int) lexical::token_type::SUB, NUM1},
    {NUM0, true, (int) lexical::token_type::PNT, NUM2},
    {NUM0, true, (int) lexical::token_type::INT, NUM3},
    {NUM1, true, (int) lexical::token_type::PNT, NUM2},
    {NUM1, true, (int) lexical::token_type::INT, NUM3},
    {NUM2, true, (int) lexical::token_type::INT, NUM5},
    {NUM3, true, (int) lexical::token_type::PNT, NUM4},
    {NUM3, true, (int) lexical::token_type::EXD, NUM6},
    {NUM4, true, (int) lexical::token_type::INT, NUM5},
    {NUM5, true, (int) lexical::token_type::EXD, NUM6},
    {NUM6, true, (int) lexical::token_type::ADD, NUM7},
    {NUM6, true, (int) lexical::token_type::SUB, NUM7},
    {NUM6, true, (int) lexical::token_type::INT, NUM8},
    {NUM7, true, (int) lexical::token_type::INT, NUM8}
};

bool FSM_num(lexical::LexicalAnalyser& lex) {
    struct state s = NUM0;
    std::cout << "<NUM>" << std::endl;
    while (true) {
        std::cout << "Estado " << s.id << ": [" << type_name(tk.type) << "] " << tk.value << std::endl;
        const struct transition *t = NULL;

        for (int i = 0; i < NUM_FSM_SIZE; i++) {
            if (num[i].current.id == s.id) {
                if ((lexical::token_type)num[i].input == tk.type) {
                    t = &num[i];
                    tk = lex.get_next();
                    break;
                }
            }
        }

        if (t) {
            s = t->next;
            continue;
        }

        if (s.accepting) {
            std::cout << "</NUM> : APROVADO" << std::endl;
            return true;
        }
        else {
            std::cout << "</NUM> : REPROVADO" << std::endl;
            return false;
        }
    }
}

int syntax_read(std::ifstream& file) {
    lexical::LexicalAnalyser lex(file);
    tk = lex.get_next();
    if (FSM_exp(lex)) {
        std::cout << "APROVADO" << std::endl;
    }
    else {
        std::cout << "REPROVADO" << std::endl;
    }
}

            // Para cada transição no reconhecedor utilizado
                // Filtra o estado da transição
                    // Verifica se a entrada corresponde a alguma transição de terminal
                        // Se sim, atribui a transição e consome próximo lexema
                    // Verifica se a entrada corresponde a alguma transição de não terminal
                        // Se sim, chama a submáquina de estados correspondente
                
            // Se houve transição realizada
                // Atualiza o estado
                // Segue o loop
            
            // Senão
                // Se o estado corrente é de aceitação
                    // Retorna true
                // Senão
                    // Retorna false

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
    }
}
