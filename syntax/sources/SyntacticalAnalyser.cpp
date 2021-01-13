#include <iostream>
#include <string>

#include "lexic.hpp"
#include "LexicalAnalyser.hpp"
#include "Debugger.hpp"
#include "syntax.hpp"

#include "SyntacticalAnalyser.hpp"

using namespace std;
using namespace syntax;

SyntacticalAnalyser::SyntacticalAnalyser(ifstream& file):
    file(file), lex(file) {}

Syntaxeme* SyntacticalAnalyser::get_next() {
    int index;

    /*Num* n = dynamic_cast<Num*>(get_eb());
    cout << "NUM: " << n->get_value() << endl;*/

    Exp* e = get_exp();

    cout << (e->is_positive()? "( " : "- ( ");
    if (e->get_operands().at(0)->get_eb_type() == Eb::NUM)
        cout << dynamic_cast<Num*>(e->get_operands().at(0))->get_value() << " ";

    for (int i = 0; i < e->get_operators().size(); i++) {
        cout << e->get_operators().at(i)->get_symbol() << " ";

        if (e->get_operands().at(i + 1)->get_eb_type() == Eb::NUM)
            cout << dynamic_cast<Num*>(e->get_operands().at(i + 1))->get_value() << " ";
    }
    cout << ")" << endl;

    /*if (consume(lexic::type::INT, false, true)) {
        int index = stoi(tk.value);

        tk = lex.get_next();

        switch (tk.type) {
            case lexic::type::LET:
                return get_assign(index);
            case lexic::type::GO:
            case lexic::type::GOTO:
                return get_goto(index);
            //case lexic::type::READ:
                //return get_read(index);
        }
    }*/
}


Assign* SyntacticalAnalyser::get_assign(int index) {
    string identifier;
    int value;

    consume(lexic::type::IDN, false, true);
    identifier = tk.value;

    consume(lexic::type::EQL, false, true);

    consume(lexic::type::INT, false, true);
    value = stoi(tk.value);

    return new Assign(index, identifier, value);
}

/*Read* SyntacticalAnalyser::get_read(int index) {
    vector<string> identifiers;
    lexic::token tk = lex.get_next();

    cout << tk.value << endl;
    if (tk.type == lexic::type::IDN)
        identifiers.push_back(tk.value);

    return new Read(index, identifiers);
}

Data* SyntacticalAnalyser::get_data(int index) {
    vector<int> values;
    lexic::token tk = lex.get_next();

    cout << tk.value << endl;
    if (tk.type == lexic::type::INT)
        values.push_back(stoi(tk.value));

    return new Data(index, values);
}*/

Goto* SyntacticalAnalyser::get_goto(int index) {
    int destination;

    if (consume(lexic::type::TO, false)) {
        consume(lexic::type::INT, false, true);
        destination = stoi(tk.value);
    }
    else {
        consume(lexic::type::INT, false, true);
        destination = stoi(tk.value);
    }

    return new Goto(index, destination);
}

Exp* SyntacticalAnalyser::get_exp() {
    bool positive = true;
    std::vector<Eb*> operands;
    std::vector<Operator*> operators;

    if (consume(lexic::type::ADD, false))
        positive = true;
    else if (consume(lexic::type::SUB, false))
        positive = false;

    operands.push_back(get_eb());

    while (true) {
        Operator* op = get_operator();

        if(op == nullptr)
            break;

        operators.push_back(op);
        operands.push_back(get_eb());
    }

    return new Exp(positive, operands, operators);
}

Operator* SyntacticalAnalyser::get_operator() {
    if (consume(lexic::type::ADD, true)) {
        consume(lexic::type::ADD, false, true);
        return new Operator(Elem::ADD, Operator::ADD, tk.value);
    }
    else if (consume(lexic::type::SUB, true)) {
        consume(lexic::type::SUB, false, true);
        return new Operator(Elem::SUB, Operator::SUB, tk.value);
    }
    else if (consume(lexic::type::MUL, true)) {
        consume(lexic::type::MUL, false, true);
        return new Operator(Elem::MUL, Operator::MUL, tk.value);
    }
    else if (consume(lexic::type::DIV, true)) {
        consume(lexic::type::DIV, false, true);
        return new Operator(Elem::DIV, Operator::DIV, tk.value);
    }
    else if (consume(lexic::type::POW, true)) {
        consume(lexic::type::POW, false, true);
        return new Operator(Elem::POW, Operator::POW, tk.value);
    }
    else {
        return nullptr;
    }
}

Eb* SyntacticalAnalyser::get_eb() {
    if (consume(lexic::type::INT, true))
        return get_num();
    else if (consume(lexic::type::IDN, true))
        return get_var();
    else
        throw syntax_exception(tk.pos, "Encontrado '" + tk.value + "' em posição inesperada");
}

/*bool SyntacticalAnalyser::get_exp(vector<Elem*>& exp) {
    vector<Elem*> exp;

    if (consume(lexic::type::ADD, false))
        exp.push_back(tk);
    else if (consume(lexic::type::SUB, false))
        exp.push_back(tk);

    if (!get_eb(exp)) throw exception();
    while(get_op(exp)) {
        if (!get_eb(exp)) throw exception();
    }

}

bool SyntacticalAnalyser::get_op(vector<Elem*>& exp) {
    if (consume(lexic::type::ADD, true)) {
        consume(lexic::type::ADD, false);
        exp.push_back(new Operator(tk));
        return true;
    }
    else if (consume(lexic::type::SUB, true)) {
        consume(lexic::type::SUB, false);
        exp.push_back(new Operator(tk));
        return true;
    }
    else if (consume(lexic::type::MUL, true)) {
        consume(lexic::type::MUL, false);
        exp.push_back(new Operator(tk));
        return true;
    }
    else if (consume(lexic::type::DIV, true)) {
        consume(lexic::type::DIV, false);
        exp.push_back(new Operator(tk));
        return true;
    }
    else if (consume(lexic::type::POW, true)) {
        consume(lexic::type::POW, false);
        exp.push_back(new Elem(Elem::type::POW));
        return true;
    }
    return false;
}

bool SyntacticalAnalyser::get_eb(vector<Elem*>& exp) {

    if (consume(lexic::type::INT, true)) {
        consume(lexic::type::INT, false);
        exp.push_back(get_num());
        return true;
    }
    else if (consume(lexic::type::IDN, true)) {
        consume(lexic::type::IDN, false);
        exp.push_back(get_var());
        return true;
    }
    else if (consume(lexic::type::PRO, true)) {
        consume(lexic::type::PRO, false, true);
        exp.push_back(new Elem(Elem::type::PRO));

        bool ok = get_exp(exp);

        if (ok) {
            consume(lexic::type::PRC, false, true);
            exp.push_back(new Elem(Elem::type::PRC));
        }

        return ok;
    }
    else if (consume(lexic::type::FN)) {}
    else if (consume(lexic::type::FN)) {}
    false;
}*/

Num* SyntacticalAnalyser::get_num() {
    int integer, exponent = 0;
    bool neg_exp = false;

    consume(lexic::type::INT, false, true);
    integer = stoi(tk.value);

    if (consume(lexic::type::EXD, false)) {
        if (consume(lexic::type::ADD, false))
            neg_exp = false;

        else if (consume(lexic::type::SUB, false))
            neg_exp = true;

        consume(lexic::type::INT, false, true);
        exponent = stoi(tk.value);
    }

    return new Num(Elem::NUM, integer, neg_exp, exponent);
}

Var* SyntacticalAnalyser::get_var() {
    string identifier;

    if (consume(lexic::type::IDN, false, true))
        identifier = tk.value;

    return new Var(Elem::VAR, identifier);
}

bool SyntacticalAnalyser::consume(lexic::type type, bool lookahead, bool force) {
    if (token_consumed)
        tk = lex.get_next();

    bool match = (tk.type == type) ? true : false;
    token_consumed = (!lookahead && match);

    if (force && !token_consumed)
        throw syntax_exception(tk.pos, string("Token inesperado: ") + tk.value);

    return match;
}

/*using namespace syntax;

lexic::token tk;

const struct syntax::state PROGRAM    = { 0, false};
const struct syntax::state STATEMENT = { 1, false};
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
    {PROGRAM,    true,  (int) lexic::type::INT, STATEMENT},
    // Bloco ASSIGN
    {STATEMENT,  true,  (int) lexic::type::LET,  ASSIGN0},
    {ASSIGN0,    true,  (int) lexic::type::IDN,  ASSIGN1},
    {ASSIGN1,    true,  (int) lexic::type::EQL,  ASSIGN2},
    {ASSIGN2,    false, (int) syntax::type::EXP, ASSIGN3},
    {ASSIGN3,    true,  (int) lexic::type::INT,  STATEMENT},
    // Bloco READ
    {STATEMENT,  true,  (int) lexic::type::READ, READ0},
    {READ0,      false, (int) syntax::type::VAR, READ1},
    {READ1,      true,  (int) lexic::type::COM,  READ2},
    {READ1,      true,  (int) lexic::type::INT,  STATEMENT},
    {READ2,      false, (int) syntax::type::VAR, READ1},
    // Bloco DATA
    {STATEMENT,  true,  (int) lexic::type::DATA, DATA0},
    {DATA0,      false, (int) syntax::type::NUM, DATA1},
    {DATA1,      true,  (int) lexic::type::COM,  DATA2},
    {DATA1,      true,  (int) lexic::type::INT,  STATEMENT},
    {DATA2,      false, (int) syntax::type::NUM, DATA1},
    // Bloco PRINT
    {STATEMENT,  true,  (int) lexic::type::PRINT, PRINT0},
    {PRINT0,     false, (int) syntax::type::EXP,  PRINT1},
    {PRINT0,     true,  (int) lexic::type::STR,   PRINT2},
    {PRINT0,     true,  (int) lexic::type::INT,   STATEMENT},
    {PRINT1,     true,  (int) lexic::type::COM,   PRINT0},
    {PRINT1,     true,  (int) lexic::type::INT,   STATEMENT},
    {PRINT2,     true,  (int) lexic::type::COM,   PRINT0},
    {PRINT2,     true,  (int) lexic::type::INT,   STATEMENT},
    {PRINT2,     false, (int) syntax::type::EXP,  PRINT1},
    // Bloco GOTO
    {STATEMENT,  true,  (int) lexic::type::GO,   GOTO0},
    {STATEMENT,  true,  (int) lexic::type::GOTO, GOTO1},
    {GOTO0,      true,  (int) lexic::type::TO,   GOTO1},
    {GOTO1,      true,  (int) lexic::type::INT,  GOTO2},
    {GOTO2,      true,  (int) lexic::type::INT,  STATEMENT},
    // Bloco IF
    {STATEMENT,  true,  (int) lexic::type::IF,   IF0},
    {IF0,        false, (int) syntax::type::EXP, IF1},
    {IF1,        true,  (int) lexic::type::GEQ,  IF2},
    {IF1,        true,  (int) lexic::type::GTN,  IF2},
    {IF1,        true,  (int) lexic::type::NEQ,  IF2},
    {IF1,        true,  (int) lexic::type::LTN,  IF2},
    {IF1,        true,  (int) lexic::type::LEQ,  IF2},
    {IF1,        true,  (int) lexic::type::EQL,  IF2},
    {IF2,        false, (int) syntax::type::EXP, IF3},
    {IF3,        true,  (int) lexic::type::THEN, IF4},
    {IF4,        true,  (int) lexic::type::INT,  IF5},
    {IF5,        true,  (int) lexic::type::INT,  STATEMENT},
    // Bloco FOR
    {STATEMENT,  true,  (int) lexic::type::FOR,  FOR0},
    {FOR0,       true,  (int) lexic::type::IDN,  FOR1},
    {FOR1,       true,  (int) lexic::type::EQL,  FOR2},
    {FOR2,       false, (int) syntax::type::EXP, FOR3},
    {FOR3,       true,  (int) lexic::type::TO,   FOR4},
    {FOR4,       false, (int) syntax::type::EXP, FOR5},
    {FOR5,       true,  (int) lexic::type::STEP, FOR6},
    {FOR5,       true,  (int) lexic::type::INT,  STATEMENT},
    {FOR6,       false, (int) syntax::type::EXP, FOR7},
    {FOR7,       true,  (int) lexic::type::INT,  STATEMENT},
    // Bloco NEXT
    {STATEMENT, true,  (int) lexic::type::NEXT, NEXT0},
    {NEXT0,      true,  (int) lexic::type::IDN,  NEXT1},
    {NEXT1,      true,  (int) lexic::type::INT,  STATEMENT},
    // Bloco DIM
    {STATEMENT,  true,  (int) lexic::type::DIM, DIM0},
    {DIM0,       true,  (int) lexic::type::IDN, DIM1},
    {DIM1,       true,  (int) lexic::type::PRO, DIM2},
    {DIM2,       true,  (int) lexic::type::INT, DIM3},
    {DIM3,       true,  (int) lexic::type::PRC, DIM4},
    {DIM4,       true,  (int) lexic::type::COM, DIM5},
    {DIM4,       true,  (int) lexic::type::INT, STATEMENT},
    {DIM5,       true,  (int) lexic::type::IDN, DIM1},
    // Bloco DEF
    {STATEMENT,  true,  (int) lexic::type::DEF,  DEF0},
    {DEF0,       true,  (int) lexic::type::FN,   DEF1},
    {DEF1,       true,  (int) lexic::type::IDN,  DEF2},
    {DEF2,       true,  (int) lexic::type::PRO,  DEF3},
    {DEF3,       true,  (int) lexic::type::IDN,  DEF4},
    {DEF4,       true,  (int) lexic::type::PRC,  DEF5},
    {DEF5,       true,  (int) lexic::type::EQL,  DEF6},
    {DEF6,       false, (int) syntax::type::EXP, DEF7},
    {DEF7,       true,  (int) lexic::type::INT,  STATEMENT},
    // Bloco GOSUB
    {STATEMENT,  true,  (int) lexic::type::GOSUB, GOSUB0},
    {GOSUB0,     true,  (int) lexic::type::INT,   GOSUB1},
    {GOSUB1,     true,  (int) lexic::type::INT,   STATEMENT},
    // Bloco RETURN
    {STATEMENT,  true,  (int) lexic::type::RETURN, RETURN},
    {RETURN,     true,  (int) lexic::type::INT,    STATEMENT},
    // Bloco REM
    {STATEMENT,  true,  (int) lexic::type::REM, REMARK0},
    {REMARK0,    true,  (int) lexic::type::CMT, REMARK1},
    {REMARK1,    true,  (int) lexic::type::INT, STATEMENT},
    // Bloco END
    {STATEMENT,  true,  (int) lexic::type::END, END},
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
                        //std::cout << "\tPRG NEXT: " << type_name(tk.type) << ": " << tk.value << std::endl;
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
    {EXP0, false, (int) syntax::type::EB, EXP1},
    {EXP1, true,  (int) lexic::type::ADD, EXP2},
    {EXP1, true,  (int) lexic::type::SUB, EXP2},
    {EXP1, true,  (int) lexic::type::MUL, EXP2},
    {EXP1, true,  (int) lexic::type::DIV, EXP2},
    {EXP1, true,  (int) lexic::type::POW, EXP2},
    {EXP2, false, (int) syntax::type::EB, EXP1},
};

bool FSM_exp(lexic::LexicalAnalyser& lex) {
    struct syntax::state s = EXP0;
    while (true) {
        const struct transition *t = nullptr;
        Debugger::print_exp(s);
        std::cout << "\tTOKEN " << type_name(tk.type) << std::endl;
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
    {EB0, false, (int) syntax::type::VAR,  EB5},
    {EB0, false, (int) syntax::type::NUM,  EB5},
    {EB1, true,  (int) syntax::type::EXP,  EB2},
    {EB2, true,  (int) lexic::type::PRC,   EB5},
    {EB3, true,  (int) lexic::type::PRO,   EB1},
    {EB4, true,  (int) lexic::type::IDN,   EB3}    
};

bool FSM_eb(lexic::LexicalAnalyser& lex) {
    struct syntax::state s = EB0;
    while (true) {
        const struct transition *t = nullptr;
        Debugger::print_eb(s);
        std::cout << "\tTOKEN " << type_name(tk.type) << std::endl;
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
    {VAR0, true,  (int) lexic::type::IDN,  VAR1},
    {VAR1, true,  (int) lexic::type::PRO,  VAR2},
    {VAR2, false, (int) syntax::type::EXP, VAR3},
    {VAR3, true,  (int) lexic::type::COM,  VAR2},
    {VAR3, true,  (int) lexic::type::PRC,  VAR4},
};

bool FSM_var(lexic::LexicalAnalyser& lex) {
    struct syntax::state s = VAR0;
    while (true) {
        const struct transition *t = nullptr;
        Debugger::print_var(s);
        std::cout << "\tTOKEN " << type_name(tk.type) << std::endl;
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

void update_number(Number& n, lexic::token& t, struct syntax::state& s) {
    if (s == NUM0 && t.type == lexic::type::SUB)
        n.negative = true;
    else if ((s == NUM0 || s == NUM1) && (t.type == lexic::type::INT))
        n.integer = std::stoi(t.value);
    else if ((s == NUM2 || s == NUM4) && (t.type == lexic::type::INT))
        n.fraction = std::stoi(t.value);
    else if (s == NUM6 && t.type == lexic::type::SUB) 
        n.exponent_negative = true;
    else if ((s == NUM6 || s == NUM7) && (t.type == lexic::type::INT))
        n.exponent = std::stoi(t.value);
}

bool FSM_num(lexic::LexicalAnalyser& lex) {
    struct syntax::state s = NUM0;
    Number number;

    while (true) {
        const struct transition *t = nullptr;
        Debugger::print_num(s);
        std::cout << "\tTOKEN " << type_name(tk.type) << std::endl;
        for (int i = 0; i < NUM_FSM_SIZE; i++) {
            if (s == num[i].current) {
                if ((lexic::type)num[i].input == tk.type) {
                    t = &num[i];
                    update_number(number, tk, s);
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
            //std::cout << "\t\t\tNUMERO: " << number.generate() << std::endl;
            return true;
        }
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
}*/
