#include <iostream>
#include <utility>
#include <vector>
#include <set>

#include "syntax.hpp"

#include "SemanticAnalyser.hpp"

using namespace std;
using namespace syntax;
using namespace semantic;

SemanticAnalyser::SemanticAnalyser(ifstream& input, ofstream& output):
    stx(input), gen(output, symb_table)
{}

void SemanticAnalyser::get_next() {
    BStatement* sx;

    auto cmp = [](BStatement* a, BStatement* b) {
        return a->get_index() < b->get_index();
    };

    set<BStatement*, decltype(cmp)> statements(cmp);

    while (true) {
        sx = stx.get_next();
        if (sx == nullptr)
            break;
        statements.insert(sx);
    }

    cout << "ACHEI " << statements.size() << " COMANDOS" << endl;

    for (auto command : statements) {
        if (command == nullptr)
            cout << "sintaxema nulo?!" << endl;

        if (Assign* assign = dynamic_cast<Assign*>(command)) {
            process_assign(assign);
        }
        else if (Read* read = dynamic_cast<Read*>(command)) {
            process_read(read);
        }
        else if (Data* data = dynamic_cast<Data*>(command)) {
            process_data(data);
        }
        else if (Goto* go = dynamic_cast<Goto*>(command)) {
            process_goto(go);
        }
        else {
            cout << "É outra coisa" << endl;
        }
    }
}

void SemanticAnalyser::process_assign(syntax::Assign* assign) {
    cout << "ASSIGN" << endl;
    process_variable(assign->get_variable());
    vector<Elem*> exp = process_expression(assign->get_expression());
    gen.generate(assign, exp);
}

void SemanticAnalyser::process_read(syntax::Read* read) {
    cout << "READ";
    for (auto var: read->get_variables()) {
        read_variables.push(var);
        process_variable(var);
        cout << " " << var->get_identifier();
    }
    cout << endl;
}

void SemanticAnalyser::process_data(syntax::Data* data) {
    cout << "DATA";
    for (auto val: data->get_values()) {
        data_values.push(val);
        cout << " " << val->get_value();
    }
    cout << endl;

    vector<pair<Var*,Num*>> read_data;
    //cout << "ATRIBUIÇÕES" << endl;
    while (!read_variables.empty() && !data_values.empty()) {
        Var* var = read_variables.front();
        Num* val = data_values.front();

        //cout << "\tVAR[" << var->get_index() << "] " << var->get_identifier() << " = " << val->get_value() << endl;

        read_data.push_back(make_pair(var, val));

        read_variables.pop();
        data_values.pop();
    }
    //cout << endl;

    gen.generate(data, read_data);
}


void SemanticAnalyser::process_goto(Goto* go) {
    cout << "GOTO";
    cout << " " << go->get_destination() << endl;
    gen.generate(go);
}

string read_elem_type(syntax::Elem* e) {
    switch (e->get_elem_type()) {
        case syntax::Elem::NUM: return to_string(dynamic_cast<syntax::Num*>(e)->get_value());
        case syntax::Elem::VAR: return dynamic_cast<syntax::Var*>(e)->get_identifier();
        case syntax::Elem::FUN: return dynamic_cast<syntax::Call*>(e)->get_identifier();
        case syntax::Elem::ADD: return "+";
        case syntax::Elem::SUB: return "-";
        case syntax::Elem::MUL: return "*";
        case syntax::Elem::DIV: return "/";
        case syntax::Elem::POW: return "^";
        case syntax::Elem::PRO: return "(";
        case syntax::Elem::PRC: return ")";
        case syntax::Elem::COM: return ",";
    }
}

void print_exp(const vector<syntax::Elem*>& exp) {
    cout << "[ ";
    for (auto elem : exp) {
        cout << read_elem_type(elem) << " ";
    }
    cout << "]" << endl;
}

vector<Elem*> SemanticAnalyser::process_expression(Exp* e) {
    //syntax::Exp* e = stx.parse_exp();

    vector<syntax::Elem*> exp;
    gen_exp_vector(e, exp);

    //print_exp(exp);

    return convert_to_postfix(exp);
}

void SemanticAnalyser::process_variable(Var* v) {
    int index = symb_table.insert_variable(v);
    //cout << "Variável gravada com índice: " << index << endl;
    v->set_index(index);
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
    gen_exp_vector_operand(operand, exp);

    int size = operators.size();
    for (int i = 0; i < size; i++) {
        exp.push_back(operators.at(i));

        Eb* operand = operands.at(i + 1);
        gen_exp_vector_operand(operand, exp);
    }
}

void SemanticAnalyser::gen_exp_vector_operand(syntax::Eb* operand, vector<syntax::Elem*>& exp) {
    if (operand->get_eb_type() == Eb::EXP) {
        exp.push_back(new Elem(Elem::PRO));
        gen_exp_vector(dynamic_cast<Exp*>(operand), exp);
        exp.push_back(new Elem(Elem::PRC));
    }
    else if (operand->get_eb_type() == Eb::CALL) {
        exp.push_back(operand);
        exp.push_back(new Elem(Elem::PRO));
        vector<Exp*> args = dynamic_cast<Call*>(operand)->get_args();
        if (!args.empty()) {
            gen_exp_vector(args.at(0), exp);
            for (int i = 1; i < args.size(); i++) {
                exp.push_back(new Elem(Elem::COM));
                gen_exp_vector(args.at(i), exp);
            }
        }
        exp.push_back(new Elem(Elem::PRC));
    }
    else {
        exp.push_back(operand);
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


vector<syntax::Elem*> SemanticAnalyser::convert_to_postfix(vector<syntax::Elem*>& infix) {
    // Based on shunting yard algorithm by Edsger Dijkstra

    vector<syntax::Elem*> postfix, stack;

    /*cout << "infix: ";
    print_exp(infix);
    cout << "postfix: ";
    print_exp(postfix);
    cout << "stack: ";
    print_exp(stack);
    cout << endl;*/

    while (!infix.empty()) {
        syntax::Elem* e = infix.front();
        infix.erase(infix.begin());
        //cout << "LIDO: " << read_elem_type(e);
        //print_exp(infix);

        if (e->get_elem_type() == syntax::Elem::NUM || e->get_elem_type() == syntax::Elem::VAR) {
            postfix.push_back(e);
        }
        else if (e->get_elem_type() == Elem::FUN) {
            stack.push_back(e);
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
        else if (e->get_elem_type() == syntax::Elem::COM) {
            while (stack.back()->get_elem_type() != syntax::Elem::PRO) {
                postfix.push_back(stack.back());
                stack.pop_back();
            }
        }
        else if (e->get_elem_type() == syntax::Elem::PRC) {
            while (stack.back()->get_elem_type() != syntax::Elem::PRO) {
                postfix.push_back(stack.back());
                stack.pop_back();
            }
            if (stack.back()->get_elem_type() == syntax::Elem::PRO) {
                stack.pop_back();
            }
            if (stack.back()->get_elem_type() == syntax::Elem::FUN) {
                postfix.push_back(stack.back());
                stack.pop_back();
            }
        }

        /*cout << "infix: ";
        print_exp(infix);
        cout << "postfix: ";
        print_exp(postfix);
        cout << "stack: ";
        print_exp(stack);
        cout << endl;*/
    }
    while (!stack.empty()) {
        postfix.push_back(stack.back());
        stack.pop_back();

        /*cout << "infix: ";
        print_exp(infix);
        cout << "postfix: ";
        print_exp(postfix);
        cout << "stack: ";
        print_exp(stack);
        cout << endl;*/
    }
    //print_exp(postfix);
    return postfix;
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