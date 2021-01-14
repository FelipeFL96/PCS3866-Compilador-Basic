#include "syntax.hpp"

#include "SemanticAnalyser.hpp"

using namespace std;
using namespace syntax;

SemanticAnalyser::SemanticAnalyser(ifstream& file):
    stx(file)
{}


string read_elem_type(syntax::Elem* e) {
    switch (e->get_elem_type()) {
        case syntax::Elem::NUM: return to_string(dynamic_cast<syntax::Num*>(e)->get_value());
        case syntax::Elem::VAR: return dynamic_cast<syntax::Var*>(e)->get_identifier();
        case syntax::Elem::ADD: return "+";
        case syntax::Elem::SUB: return "-";
        case syntax::Elem::MUL: return "*";
        case syntax::Elem::DIV: return "/";
        case syntax::Elem::POW: return "^";
        case syntax::Elem::PRO: return "(";
        case syntax::Elem::PRC: return ")";
    }
}

void print_exp(const vector<syntax::Elem*>& exp) {
    cout << "[ ";
    for (auto elem : exp) {
        cout << read_elem_type(elem) << " ";
    }
    cout << "]" << endl;
}

void SemanticAnalyser::parse_expression() {
    syntax::Exp* e = stx.get_exp();

    vector<syntax::Elem*> exp;
    gen_exp_vector(e, exp);

    print_exp(exp);

    convert_to_postfix(exp);
}

void SemanticAnalyser::gen_exp_vector(syntax::Exp* e, vector<syntax::Elem*>& exp) {

    if (e->is_negative()) {
        gen_negative_exp_vector(e, exp);
        return;
    }

    vector<Eb*> operands = e->get_operands();
    vector<Operator*> operators = e->get_operators();

    //TODO: Retornar erro se não houver operandos
    Eb* operand = operands.front();
    if (operand->get_eb_type() == Eb::EXP) {
        exp.push_back(new Elem(Elem::PRO));
        gen_exp_vector(dynamic_cast<Exp*>(operand), exp);
        exp.push_back(new Elem(Elem::PRC));
    }
    else {
        exp.push_back(operand);
    }

    int size = operators.size();
    for (int i = 0; i < size; i++) {
        exp.push_back(operators.at(i));

        Eb* operand = operands.at(i + 1);
        if (operand->get_eb_type() == Eb::EXP) {
            exp.push_back(new Elem(Elem::PRO));
            gen_exp_vector(dynamic_cast<Exp*>(operand), exp);
            exp.push_back(new Elem(Elem::PRC));
        }
        else {
            exp.push_back(operand);
        }
    }
}

void SemanticAnalyser::gen_negative_exp_vector(syntax::Exp* e, std::vector<syntax::Elem*>& exp) {
    vector<Operator*> operators;
    Operator* sub = new Operator(Elem::SUB, Operator::SUB, "-");
    operators.push_back(sub);

    vector<Eb*> operands;
    Eb* zero = new Num(Elem::NUM, 0, false, 0);
    operands.push_back(zero);
    e->make_positive();
    operands.push_back(e);

    Exp* neg_exp = new Exp(Elem::EXP, false, operands, operators);

    gen_exp_vector(neg_exp, exp);
}

int precedence(syntax::Elem* e) {
    switch (e->get_elem_type()) {
        case syntax::Elem::ADD: 
            return 2;
        case syntax::Elem::SUB: 
            return 2;
        case syntax::Elem::MUL: 
            return 3;
        case syntax::Elem::DIV: 
            return 3;
        case syntax::Elem::POW: 
            return 4;
        default:
            return 0;
    }
}

enum assoc {
    LEFT,
    RIGHT
};

int associativity(syntax::Elem* e) {
    switch (e->get_elem_type()) {
        case syntax::Elem::ADD: 
            return LEFT;
        case syntax::Elem::SUB: 
            return LEFT;
        case syntax::Elem::MUL: 
            return LEFT;
        case syntax::Elem::DIV: 
            return LEFT;
        case syntax::Elem::POW: 
            return RIGHT;
        default:
            return LEFT;
    }
}


void SemanticAnalyser::convert_to_postfix(vector<syntax::Elem*>& infix) {
    // Based on shunting yard algorithm by Edsger Dijkstra

    vector<syntax::Elem*> postfix, stack;

    cout << "infix: ";
    print_exp(infix);
    cout << "postfix: ";
    print_exp(postfix);
    cout << "stack: ";
    print_exp(stack);
    cout << endl;

    while (!infix.empty()) {
        syntax::Elem* e = infix.front();
        infix.erase(infix.begin());
        //cout << "LIDO: " << read_elem_type(e);
        //print_exp(infix);

        if (e->get_elem_type() == syntax::Elem::NUM || e->get_elem_type() == syntax::Elem::VAR) {
            postfix.push_back(e);
        }
        else if (e->is_operator()) {
            while ( !stack.empty()
                && (stack.back()->is_operator())
                && ((precedence(stack.back()) > precedence(e)) || (precedence(stack.back()) == precedence(e) && associativity(e) == LEFT))
                && (stack.back()->get_elem_type() != syntax::Elem::PRO)
                ) {
                postfix.push_back(stack.back());
                stack.pop_back();
            }
            stack.push_back(e);
        }
        else if (e->get_elem_type() == syntax::Elem::PRO) {
            stack.push_back(e);
        }
        else if (e->get_elem_type() == syntax::Elem::PRC) {
            while (stack.back()->get_elem_type() != syntax::Elem::PRO) {
                postfix.push_back(stack.back());
                stack.pop_back();
            }
            if (stack.back()->get_elem_type() == syntax::Elem::PRO) {
                stack.pop_back();
            }
        }

        cout << "infix: ";
        print_exp(infix);
        cout << "postfix: ";
        print_exp(postfix);
        cout << "stack: ";
        print_exp(stack);
        cout << endl;
    }
    while (!stack.empty()) {
        postfix.push_back(stack.back());
        stack.pop_back();

        cout << "infix: ";
        print_exp(infix);
        cout << "postfix: ";
        print_exp(postfix);
        cout << "stack: ";
        print_exp(stack);
        cout << endl;
    }
    /*
    while there are tokens to be read:
        read a token.
        if the token is a number, then:
            push it to the output queue.
        else if the token is a function then:
            push it onto the operator stack 
        else if the token is an operator then:
            while ((there is an operator at the top of the operator stack)
                and ((the operator at the top of the operator stack has greater precedence)
                    or (the operator at the top of the operator stack has equal precedence and the token is left associative))
                and (the operator at the top of the operator stack is not a left parenthesis)):
                pop operators from the operator stack onto the output queue.
            push it onto the operator stack.
        else if the token is a left parenthesis (i.e. "("), then:
            push it onto the operator stack.
        else if the token is a right parenthesis (i.e. ")"), then:
            while the operator at the top of the operator stack is not a left parenthesis:
                pop the operator from the operator stack onto the output queue.
            // If the stack runs out without finding a left parenthesis, then there are mismatched parentheses.
            if there is a left parenthesis at the top of the operator stack, then:
                pop the operator from the operator stack and discard it
            if there is a function token at the top of the operator stack, then:
                pop the function from the operator stack onto the output queue.
    // After while loop, if operator stack not null, pop everything to output queue
    if there are no more tokens to read then:
        while there are still operator tokens on the stack:
            // If the operator token on the top of the stack is a parenthesis, then there are mismatched parentheses.
            pop the operator from the operator stack onto the output queue.
    exit.
    */
}