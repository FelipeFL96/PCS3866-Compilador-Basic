#include <iostream>
#include <string>

#include "lexical.hpp"
#include "LexicalAnalyser.hpp"
#include "Debugger.hpp"

#include "SyntacticalAnalyser.hpp"

using namespace syntax;

lexic::token tk;

const struct syntax::state PROGRAM    = { 0, false};
const struct syntax::state STATEMENT0 = { 1, false};
const struct syntax::state ASSIGN0    = { 2, false};
const struct syntax::state ASSIGN1    = { 3, false};
const struct syntax::state ASSIGN2    = { 4, false};
const struct syntax::state ASSIGN3    = { 5, false};
const struct syntax::state READ0      = { 6, false};
const struct syntax::state READ1      = { 7, false};
const struct syntax::state READ2      = { 8, false};
const struct syntax::state DATA0      = { 9, false};
const struct syntax::state DATA1      = {10, false};
const struct syntax::state DATA2      = {11, false};
const struct syntax::state PRINT0     = {12, false};
const struct syntax::state PRINT1     = {13, false};
const struct syntax::state PRINT2     = {14, false};
const struct syntax::state GOTO0      = {15, false};
const struct syntax::state GOTO1      = {16, false};
const struct syntax::state GOTO2      = {17, false};
const struct syntax::state IF0        = {18, false};
const struct syntax::state IF1        = {19, false};
const struct syntax::state IF2        = {20, false};
const struct syntax::state IF3        = {21, false};
const struct syntax::state IF4        = {22, false};
const struct syntax::state IF5        = {23, false};
const struct syntax::state FOR0       = {24, false};
const struct syntax::state FOR1       = {25, false};
const struct syntax::state FOR2       = {36, false};
const struct syntax::state FOR3       = {37, false};
const struct syntax::state FOR4       = {38, false};
const struct syntax::state FOR5       = {39, false};
const struct syntax::state FOR6       = {30, false};
const struct syntax::state FOR7       = {31, false};
const struct syntax::state NEXT0      = {32, false};
const struct syntax::state NEXT1      = {33, false};
const struct syntax::state DIM0       = {34, false};
const struct syntax::state DIM1       = {35, false};
const struct syntax::state DIM2       = {46, false};
const struct syntax::state DIM3       = {47, false};
const struct syntax::state DIM4       = {48, false};
const struct syntax::state DIM5       = {49, false};
const struct syntax::state DEF0       = {40, false};
const struct syntax::state DEF1       = {41, false};
const struct syntax::state DEF2       = {42, false};
const struct syntax::state DEF3       = {43, false};
const struct syntax::state DEF4       = {44, false};
const struct syntax::state DEF5       = {45, false};
const struct syntax::state DEF6       = {56, false};
const struct syntax::state DEF7       = {57, false};
const struct syntax::state GOSUB0     = {58, false};
const struct syntax::state GOSUB1     = {59, false};
const struct syntax::state RETURN     = {50, false};
const struct syntax::state REMARK0    = {51, false};
const struct syntax::state REMARK1    = {52, false};
const struct syntax::state END        = {53, true};

#define PROGRAM_FSM_SIZE 81
const struct transition program[PROGRAM_FSM_SIZE] = {
    {PROGRAM,    true,  (int) lexic::type::INT, STATEMENT0},
    // Bloco ASSIGN
    {STATEMENT0, true,  (int) lexic::type::LET, ASSIGN0},
    {ASSIGN0,    true,  (int) lexic::type::IDN, ASSIGN1},
    {ASSIGN1,    true,  (int) lexic::type::EQL, ASSIGN2},
    {ASSIGN2,    false, (int) syntax::type::EXP,        ASSIGN3},
    {ASSIGN3,    true,  (int) lexic::type::INT, STATEMENT0},
    // Bloco READ
    {STATEMENT0, true,  (int) lexic::type::READ, READ0},
    {READ0,      false, (int) syntax::type::VAR,         READ1},
    {READ1,      true,  (int) lexic::type::COM,  READ2},
    {READ1,      true,  (int) lexic::type::INT,  STATEMENT0},
    {READ2,      false, (int) syntax::type::VAR,         READ1},
    // Bloco DATA
    {STATEMENT0, true,  (int) lexic::type::DATA, DATA0},
    {DATA0,      false, (int) syntax::type::NUM,         DATA1},
    {DATA1,      true,  (int) lexic::type::COM,  DATA2},
    {DATA1,      true,  (int) lexic::type::INT,  STATEMENT0},
    {DATA2,      false, (int) syntax::type::NUM,         DATA1},
    // Bloco PRINT
    {STATEMENT0, true,  (int) lexic::type::PRINT, PRINT0},
    {PRINT0,     false, (int) syntax::type::EXP,          PRINT1},
    {PRINT0,     true,  (int) lexic::type::STR,   PRINT2},
    {PRINT0,     true,  (int) lexic::type::INT,   STATEMENT0},
    {PRINT1,     true,  (int) lexic::type::COM,   PRINT0},
    {PRINT1,     true,  (int) lexic::type::INT,   STATEMENT0},
    {PRINT2,     true,  (int) lexic::type::COM,   PRINT0},
    {PRINT2,     true,  (int) lexic::type::INT,   STATEMENT0},
    {PRINT2,     false, (int) syntax::type::EXP,          PRINT1},
    // Bloco GOTO
    {STATEMENT0, true,  (int) lexic::type::GO,   GOTO0},
    {STATEMENT0, true,  (int) lexic::type::GOTO, GOTO1},
    {GOTO0,      true,  (int) lexic::type::TO,   GOTO1},
    {GOTO1,      true,  (int) lexic::type::INT,  GOTO2},
    {GOTO2,      true,  (int) lexic::type::INT,  STATEMENT0},
    // Bloco IF
    {STATEMENT0, true,  (int) lexic::type::IF,   IF0},
    {IF0,        false, (int) syntax::type::EXP,         IF1},
    {IF1,        true,  (int) lexic::type::GEQ,  IF2},
    {IF1,        true,  (int) lexic::type::GTN,  IF2},
    {IF1,        true,  (int) lexic::type::NEQ,  IF2},
    {IF1,        true,  (int) lexic::type::LTN,  IF2},
    {IF1,        true,  (int) lexic::type::LEQ,  IF2},
    {IF1,        true,  (int) lexic::type::EQL,  IF2},
    {IF2,        false, (int) syntax::type::EXP,         IF3},
    {IF3,        true,  (int) lexic::type::THEN, IF4},
    {IF4,        true,  (int) lexic::type::INT,  IF5},
    {IF5,        true,  (int) lexic::type::INT,  STATEMENT0},
    // Bloco FOR
    {STATEMENT0, true,  (int) lexic::type::FOR,  FOR0},
    {FOR0,       true,  (int) lexic::type::IDN,  FOR1},
    {FOR1,       true,  (int) lexic::type::EQL,  FOR2},
    {FOR2,       false, (int) syntax::type::EXP,         FOR3},
    {FOR3,       true,  (int) lexic::type::TO,   FOR4},
    {FOR4,       false, (int) syntax::type::EXP,         FOR5},
    {FOR5,       true,  (int) lexic::type::STEP, FOR6},
    {FOR5,       true,  (int) lexic::type::INT,  STATEMENT0},
    {FOR6,       false, (int) syntax::type::EXP,         FOR7},
    {FOR7,       true,  (int) lexic::type::INT,  STATEMENT0},
    // Bloco NEXT
    {STATEMENT0, true,  (int) lexic::type::NEXT, NEXT0},
    {NEXT0,      true,  (int) lexic::type::IDN,  NEXT1},
    {NEXT1,      true,  (int) lexic::type::INT,  STATEMENT0},
    // Bloco DIM
    {STATEMENT0, true,  (int) lexic::type::DIM, DIM0},
    {DIM0,       true,  (int) lexic::type::IDN, DIM1},
    {DIM1,       true,  (int) lexic::type::PRO, DIM2},
    {DIM2,       true,  (int) lexic::type::INT, DIM3},
    {DIM3,       true,  (int) lexic::type::PRC, DIM4},
    {DIM4,       true,  (int) lexic::type::COM, DIM5},
    {DIM4,       true,  (int) lexic::type::INT, STATEMENT0},
    {DIM5,       true,  (int) lexic::type::IDN, DIM1},
    // Bloco DEF
    {STATEMENT0, true,  (int) lexic::type::DEF, DEF0},
    {DEF0,       true,  (int) lexic::type::FN,  DEF1},
    {DEF1,       true,  (int) lexic::type::IDN, DEF2},
    {DEF2,       true,  (int) lexic::type::PRO, DEF3},
    {DEF3,       true,  (int) lexic::type::IDN, DEF4},
    {DEF4,       true,  (int) lexic::type::PRC, DEF5},
    {DEF5,       true,  (int) lexic::type::EQL, DEF6},
    {DEF6,       false, (int) syntax::type::EXP,        DEF7},
    {DEF7,       true,  (int) lexic::type::INT, STATEMENT0},
    // Bloco GOSUB
    {STATEMENT0, true,  (int) lexic::type::GOSUB, GOSUB0},
    {GOSUB0,     true,  (int) lexic::type::INT,   GOSUB1},
    {GOSUB1,     true,  (int) lexic::type::INT,   STATEMENT0},
    // Bloco RETURN
    {STATEMENT0, true,  (int) lexic::type::RETURN, RETURN},
    {RETURN,     true,  (int) lexic::type::INT, STATEMENT0},
    // Bloco REM
    {STATEMENT0, true,  (int) lexic::type::REM, REMARK0},
    {REMARK0,    true,  (int) lexic::type::CMT, REMARK1},
    {REMARK1,    true,  (int) lexic::type::INT, STATEMENT0},
    // Bloco END
    {STATEMENT0, true,  (int) lexic::type::END, END},
};

bool FSM_program(lexic::LexicalAnalyser& lex) {
    struct syntax::state s = PROGRAM;
    while (true) {
        const struct transition *t = nullptr;
        Debugger::print_program(s);
        std::cout << "\tTOKEN " << type_name(tk.type) << std::endl;
        for (int i = 0; i < PROGRAM_FSM_SIZE; i++) {
            if (program[i].current.id == s.id) {
                if (program[i].input_is_terminal) {
                    if ((lexic::type) program[i].input == tk.type ) {
                        t = &program[i];
                        tk = lex.get_next();
                        std::cout << "\tPRG NEXT: " << type_name(tk.type) << ": " << tk.value << std::endl;
                        break;
                    }
                }
                else if (
                    (type) program[i].input == type::NUM
                    && (tk.type == lexic::type::ADD || tk.type == lexic::type::SUB || tk.type == lexic::type::INT)
                    && FSM_num(lex)
                ){
                    t = &program[i];
                    break;
                }
                else if ((type)program[i].input == type::VAR && tk.type == lexic::type::IDN && FSM_var(lex)) {
                    t = &program[i];
                    break;
                }
                else if ((type)program[i].input == type::EXP && FSM_exp(lex)) {
                    t = &program[i];
                    break;
                }
            }
        }

        if (t) {
            s = t->next;
            continue;
        }

        if (s.accepting)
            return true;
        else
            return false;
    }
}

#define EXP_FSM_SIZE 9
const struct syntax::state EXP0 = {0, false};
const struct syntax::state EXP1 = {1, true};
const struct syntax::state EXP2 = {2, true};

const struct transition exp[EXP_FSM_SIZE] = {
    {EXP0, true,  (int) lexic::type::ADD, EXP0},
    {EXP0, true,  (int) lexic::type::SUB, EXP0},
    {EXP0, false, (int) type::EB,  EXP1},
    {EXP1, true,  (int) lexic::type::ADD, EXP2},
    {EXP1, true,  (int) lexic::type::SUB, EXP2},
    {EXP1, true,  (int) lexic::type::MUL, EXP2},
    {EXP1, true,  (int) lexic::type::DIV, EXP2},
    {EXP1, true,  (int) lexic::type::POW, EXP2},
    {EXP2, false, (int) type::EB,  EXP1},
};

bool FSM_exp(lexic::LexicalAnalyser& lex) {
    struct syntax::state s = EXP0;
    while (true) {
        const struct transition *t = nullptr;

        for (int i = 0; i < EXP_FSM_SIZE; i++) {
            if (exp[i].current.id == s.id) {
                if (exp[i].input_is_terminal) {
                    if ((lexic::type)exp[i].input == tk.type) {
                        t = &exp[i];
                        tk = lex.get_next();
                        break;
                    }
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

        if (s.accepting)
            return true;
        return false;
    }
}

#define EB_FSM_SIZE 18
const struct syntax::state EB0 = {0, false};
const struct syntax::state EB1 = {1, false};
const struct syntax::state EB2 = {2, false};
const struct syntax::state EB3 = {3, false};
const struct syntax::state EB4 = {4, false};
const struct syntax::state EB5 = {5, true};

struct transition eb[EB_FSM_SIZE] = {
    {EB0, true,  (int) lexic::type::PRO,   EB1},
    {EB0, true,  (int) lexic::type::FNSIN, EB3},
    {EB0, true,  (int) lexic::type::FNCOS, EB3},
    {EB0, true,  (int) lexic::type::FNTAN, EB3},
    {EB0, true,  (int) lexic::type::FNATN, EB3},
    {EB0, true,  (int) lexic::type::FNEXP, EB3},
    {EB0, true,  (int) lexic::type::FNABS, EB3},
    {EB0, true,  (int) lexic::type::FNLOG, EB3},
    {EB0, true,  (int) lexic::type::FNSQR, EB3},
    {EB0, true,  (int) lexic::type::FNINT, EB3},
    {EB0, true,  (int) lexic::type::FNRND, EB3},
    {EB0, true,  (int) lexic::type::FN,    EB4},
    {EB0, false, (int) type::VAR,                  EB5},
    {EB0, false, (int) type::NUM,                  EB5},
    {EB1, true,  (int) type::EXP,                  EB2},
    {EB2, true,  (int) lexic::type::PRC,   EB5},
    {EB3, true,  (int) lexic::type::PRO,   EB1},
    {EB4, true,  (int) lexic::type::IDN,   EB3}    
};

bool FSM_eb(lexic::LexicalAnalyser& lex) {
    struct syntax::state s = EB0;
    while (true) {
        const struct transition *t = nullptr;

        for (int i = 0; i < EB_FSM_SIZE; i++) {
            if (eb[i].current.id == s.id) {
                if (eb[i].input_is_terminal) {
                    if ((lexic::type)eb[i].input == tk.type) {
                        t = &eb[i];
                        tk = lex.get_next();
                        break;
                    }
                }
                else if (
                    (type) eb[i].input == type::NUM
                    && (tk.type == lexic::type::ADD || tk.type == lexic::type::SUB || tk.type == lexic::type::INT)
                    && FSM_num(lex)
                ){
                    t = &eb[i];
                    break;
                }
                else if ((type)eb[i].input == type::VAR && tk.type == lexic::type::IDN && FSM_var(lex)) {
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

        if (s.accepting)
            return true;
        return false;
    }
}


#define VAR_FSM_SIZE 5
const struct syntax::state VAR0 = {0, false};
const struct syntax::state VAR1 = {1, true};
const struct syntax::state VAR2 = {2, false};
const struct syntax::state VAR3 = {3, false};
const struct syntax::state VAR4 = {4, true};

const struct transition var[5] = {
    {VAR0, true,  (int) lexic::type::IDN, VAR1},
    {VAR1, true,  (int) lexic::type::PRO, VAR2},
    {VAR2, false, (int) type::EXP,                VAR3},
    {VAR3, true,  (int) lexic::type::COM, VAR2},
    {VAR3, true,  (int) lexic::type::PRC, VAR4},
};

bool FSM_var(lexic::LexicalAnalyser& lex) {
    struct syntax::state s = VAR0;
    while (true) {
        const struct transition *t = nullptr;

        for (int i = 0; i < VAR_FSM_SIZE; i++) {
            if (var[i].current.id == s.id) {
                if ((lexic::type)var[i].input == tk.type) {
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

        if (s.accepting)
            return true;
        return false;
    }
}

#define NUM_FSM_SIZE 15
const struct syntax::state NUM0 = {0, false};
const struct syntax::state NUM1 = {1, false};
const struct syntax::state NUM2 = {2, false};
const struct syntax::state NUM3 = {3, true};
const struct syntax::state NUM4 = {4, false};
const struct syntax::state NUM5 = {5, true};
const struct syntax::state NUM6 = {6, false};
const struct syntax::state NUM7 = {7, false};
const struct syntax::state NUM8 = {8, true};

const struct transition num[NUM_FSM_SIZE] = {
    {NUM0, true, (int) lexic::type::ADD, NUM1},
    {NUM0, true, (int) lexic::type::SUB, NUM1},
    {NUM0, true, (int) lexic::type::PNT, NUM2},
    {NUM0, true, (int) lexic::type::INT, NUM3},
    {NUM1, true, (int) lexic::type::PNT, NUM2},
    {NUM1, true, (int) lexic::type::INT, NUM3},
    {NUM2, true, (int) lexic::type::INT, NUM5},
    {NUM3, true, (int) lexic::type::PNT, NUM4},
    {NUM3, true, (int) lexic::type::EXD, NUM6},
    {NUM4, true, (int) lexic::type::INT, NUM5},
    {NUM5, true, (int) lexic::type::EXD, NUM6},
    {NUM6, true, (int) lexic::type::ADD, NUM7},
    {NUM6, true, (int) lexic::type::SUB, NUM7},
    {NUM6, true, (int) lexic::type::INT, NUM8},
    {NUM7, true, (int) lexic::type::INT, NUM8}
};

bool FSM_num(lexic::LexicalAnalyser& lex) {
    struct syntax::state s = NUM0;
    while (true) {
        const struct transition *t = nullptr;

        for (int i = 0; i < NUM_FSM_SIZE; i++) {
            if (num[i].current.id == s.id) {
                if ((lexic::type)num[i].input == tk.type) {
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

        if (s.accepting)
            return true;
        return false;
    }
}

int syntax_read(std::ifstream& file) {
    lexic::LexicalAnalyser lex(file);
    tk = lex.get_next();

    if (FSM_program(lex)) {
        std::cout << "\033[1;32mAPROVADO\033[0m" << std::endl;
    }
    else {
        std::cout << "\033[1;31mREPROVADO\033[0m" << std::endl;
    }
}
