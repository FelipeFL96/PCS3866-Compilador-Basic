#include <iostream>
#include <string.h>
#include <utility>
#include <vector>
#include <set>

#include "syntax.hpp"

#include "SemanticAnalyser.hpp"

using namespace std;
using namespace syntax;
using namespace semantic;


auto cmp = [](syntax::BStatement* a, syntax::BStatement* b) {
    if (a->get_index() == b->get_index()) {
        throw semantic_exception(a->get_position(), "Índice de linha já existente");
    }
    return a->get_index() < b->get_index();
};
set<syntax::BStatement*, decltype(cmp)> statements(cmp);


SemanticAnalyser::SemanticAnalyser(ifstream& input, ofstream& output):
    stx(input), gen(input, output, symb_table)
{}


int find_next_index(BStatement* current) {
    for (auto statement : statements) {
        if (statement->get_index() > current->get_index()
            && !dynamic_cast<Def*>(statement)
            && !dynamic_cast<Rem*>(statement)
            ) {
            return statement->get_index();
        }
    }
}

void SemanticAnalyser::get_next() {
    BStatement* sx;

    while (true) {
        sx = stx.get_next();

        if (dynamic_cast<Rem*>(sx))
            continue;

        if (sx == nullptr)
            break;

        statements.insert(sx);
    }

    cout << "ACHEI " << statements.size() << " COMANDOS" << endl;

    if (!dynamic_cast<End*>(*statements.rbegin()))
        throw semantic_exception((*statements.rbegin())->get_position(), "Programa não termina com comando END");

    gen.generate_header();

    bool ended = false;
    for (auto command : statements) {
        if (ended)
            throw semantic_exception(command->get_position(), "Programa continua após o comando END");

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
        else if (Print* print = dynamic_cast<Print*>(command)) {
            process_print(print);
        }
        else if (Goto* go = dynamic_cast<Goto*>(command)) {
            process_goto(go);
        }
        else if (If* ift = dynamic_cast<If*>(command)) {
            process_if(ift);
        }
        else if (For* loop = dynamic_cast<For*>(command)) {
            process_for(loop);
        }
        else if (Next* next = dynamic_cast<Next*>(command)) {
            process_next(next);
        }
        else if (Dim* dim = dynamic_cast<Dim*>(command)) {
            process_dim(dim);
        }
        else if (Def* def = dynamic_cast<Def*>(command)) {
            process_def(def);
        }
        else if (Gosub* gosub = dynamic_cast<Gosub*>(command)) {
            process_gosub(gosub);
        }
        else if (Return* ret = dynamic_cast<Return*>(command)) {
            process_return(ret);
        }
        else if (End* end = dynamic_cast<End*>(command)) {
            ended = true;
            process_end(end);
        }
        else {
            cout << "É outra coisa" << endl;
        }
    }
    gen.generate_variables();
}

void SemanticAnalyser::process_assign(syntax::Assign* assign) {
    cout << "ASSIGN" << endl;
    process_variable(assign->get_variable());
    vector<Elem*> exp = process_expression(assign->get_expression());
    int next_index = find_next_index(assign);
    gen.generate(assign, exp, next_index);
}

void SemanticAnalyser::process_read(syntax::Read* read) {
    cout << "READ" << endl;
    for (auto var: read->get_variables()) {
        if (var->is_array()) {
            if (symb_table.select_variable(var) == 0)
                throw semantic_exception(var->get_position(), "Atribuição de variável indexada não declarada '" + var->get_identifier() + "' não é permitida");

            Array* decl = dynamic_cast<Array*>(symb_table.pointer_to_variable(var));
            ArrayAccess* access = dynamic_cast<ArrayAccess*>(var);
            access->set_array(decl);

            vector<Elem*> processed_array_exps;
            processed_array_exps = process_array_access_exp(access);

            access->set_processed_access_exps(processed_array_exps);
        }

        read_variables.push(var);
        process_variable(var);
        cout << " " << var->get_identifier();
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
    int next_index = find_next_index(read);
    if (!read_data.empty())
        gen.generate(read, read_data, next_index);
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
    int next_index = find_next_index(data);
    if (!read_data.empty())
        gen.generate(data, read_data, next_index);
}

void SemanticAnalyser::process_print(syntax::Print* print) {
    cout << "Geração para PRINT não implementada" << endl;
}

void SemanticAnalyser::process_goto(Goto* go) {
    cout << "GOTO";
    cout << " " << go->get_destination() << endl;

    for (auto statement : statements) {
        if (go->get_destination() == statement->get_index()) {
            gen.generate(go);
            return;
        }
    }

    throw semantic_exception(go->get_position(), string("Comando GOTO com linha de destino inexistente"));
}

void SemanticAnalyser::process_if(syntax::If* ift) {
    cout << "IF" << endl;
    vector<Elem*> left = process_expression(ift->get_left());
    vector<Elem*> right = process_expression(ift->get_right());

    int next_index = find_next_index(ift);

    for (auto statement : statements) {
        if (ift->get_destination() == statement->get_index()) {
            gen.generate(ift, left, right, next_index);
            return;
        }
    }

    throw semantic_exception(ift->get_position(), string("Comando IF com linha de destino inexistente"));
}

void SemanticAnalyser::process_for(For* loop) {
    cout << "FOR " << endl;

    if (symb_table.select_variable(loop->get_iterator()))
        throw semantic_exception(loop->get_position(), "Variável de iteração " + loop->get_iterator()->get_identifier() + " já declarada");

    process_variable(loop->get_iterator());

    for_stack.push_back(loop);
}

void SemanticAnalyser::process_next(Next* next) {
    cout << "NEXT " << endl;

    if (for_stack.empty())
        throw semantic_exception(next->get_position(), "NEXT sem FOR correspondente");

    process_variable(next->get_iterator());

    For* loop = for_stack.back();

    if (symb_table.select_variable(next->get_iterator())
        != symb_table.select_variable(loop->get_iterator()))
        throw semantic_exception(next->get_position(), "NEXT para laço não imediatamente anterior");

    cout << loop->get_iterator()->get_identifier() << "[" << loop->get_iterator()->get_index() <<  "]" << endl;

    // Geração do FOR correspondente
    process_variable(loop->get_iterator());
    cout << loop->get_iterator()->get_identifier() << "[" << loop->get_iterator()->get_index() <<  "]" << endl;
    vector<Elem*> init = process_expression(loop->get_init());
    vector<Elem*> stop = process_expression(loop->get_stop());
    vector<Elem*> step = process_expression(loop->get_step());
    int index_inside_loop = find_next_index(loop);
    int index_outside_loop = find_next_index(next);
    gen.generate(loop, init, stop, step, index_inside_loop, index_outside_loop);
    
    // Geração do NEXT
    next->set_loop(loop);
    gen.generate(next);

    for_stack.pop_back();
}

void SemanticAnalyser::process_dim(Dim* dim) {
    cout << "DIM" << endl;

    for (auto array : dim->get_arrays()) {

        int ret = symb_table.select_variable(array);

        process_variable(array);
        /*cout << "\t" << array->get_identifier() << "[";
        for (auto dimension : array->get_dimensions()) {
            if (dimension == array->get_dimensions().at(0))
                cout << dimension;
            else
                cout << "," << dimension ;
        }
        cout << "]" << endl;*/
    }
}

void identify_def_parameters(Def* def, Exp* exp) {
    for (auto operand : exp->get_operands()) {
        if (Var* v = dynamic_cast<Var*>(operand)) {
            for (auto parameter : def->get_parameters()) {
                if (v->get_identifier() == parameter->get_identifier())
                    v->make_parameter(def->get_identifier());
            }
        }
        else if (Exp* e = dynamic_cast<Exp*>(operand)) {
            identify_def_parameters(def, e);
        }
    }
}

void SemanticAnalyser::process_def(Def* def) {
    cout << "DEF " << def->get_identifier() << ": ";
    if (!symb_table.insert_function(def))
        throw semantic_exception(def->get_position(), "Declaração dupla para função " + def->get_identifier());

    identify_def_parameters(def, def->get_exp());

    for (auto parameter : def->get_parameters()) {
        parameter->make_parameter(def->get_identifier());
        process_variable(parameter);
        cout << parameter->get_identifier() << ", ";
    }
    cout << endl;

    vector<Elem*> exp = process_expression(def->get_exp());

    gen.generate(def, exp);
}

void SemanticAnalyser::process_gosub(syntax::Gosub* gosub) {
    cout << "GOSUB" << endl;

    for (auto statement : statements) {
        if (gosub->get_destination() == statement->get_index()) {
            gen.generate(gosub);
            return;
        }
    }

    throw semantic_exception(gosub->get_position(), string("Comando GOSUB com subrotina de destino inexistente"));
}

void SemanticAnalyser::process_return(syntax::Return* ret) {
    cout << "RETURN" << endl;
    gen.generate(ret);
}

void SemanticAnalyser::process_end(syntax::End* end) {
    if (!read_variables.empty())
        throw semantic_exception(end->get_position(), "Fim de programa atingido com " + to_string(read_variables.size()) + " variável(is) aguardando em READ");
    else if (!for_stack.empty())
        throw semantic_exception(end->get_position(), "Fim de programa atingido com laço FOR não terminado");

    gen.generate(end);
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
    int index;
    if (Array* a = dynamic_cast<Array*>(v)) {
        index = symb_table.insert_array(a);
    }
    else {
        index = symb_table.insert_variable(v);
    }
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
    else if (operand->get_eb_type() == Eb::VAR) {
        exp.push_back(operand);

        Var* v = dynamic_cast<Var*>(operand);

        if (v->is_array()) {

            if (symb_table.select_variable(v) == 0)
                    throw semantic_exception(v->get_position(), "Acesso a variável indexada não declarada '" + v->get_identifier() + "' não é permitido");

            Array* decl = dynamic_cast<Array*>(symb_table.pointer_to_variable(v));
            ArrayAccess* access = dynamic_cast<ArrayAccess*>(v);
            access->set_array(decl);
            int dimensions = access->get_access_exps().size();

            exp.push_back(new Elem(Elem::PRO));

            for (int i = 0; i < dimensions; i++) {
                if (i != 0)
                    exp.push_back(new Elem(Elem::ADD));

                exp.push_back(new Elem(Elem::PRO));
                gen_exp_vector(access->get_access_exps().at(i), exp);
                exp.push_back(new Elem(Elem::PRC));

                for ( int j = i + 1; j < dimensions; j++) {
                    exp.push_back(new Elem(Elem::MUL));
                    exp.push_back(new Num(Elem::NUM, decl->get_dimensions().at(j), 0, false, 0));
                }
            }

            exp.push_back(new Elem(Elem::PRC));
        }
        else {
            exp.push_back(operand);
        }
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
    Eb* zero = new Num(Elem::NUM, 0, 0, false, 0);
    operands.push_back(zero);
    e->make_positive();
    operands.push_back(e);

    Exp* neg_exp = new Exp(Elem::EXP, false, operands, operators);

    gen_exp_vector(neg_exp, exp);
}

vector<Elem*> SemanticAnalyser::process_array_access_exp(ArrayAccess* access) {
    vector<Elem*> processed_access_exps;

    int dimensions = access->get_access_exps().size();
    Array* decl = access->get_array();

    for (int i = 0; i < dimensions; i++) {
        vector<Elem*> ae = process_expression(access->get_access_exps().at(i));
        for (auto elem : ae) {
            processed_access_exps.push_back(elem);
        }

        for ( int j = i + 1; j < dimensions; j++) {
            processed_access_exps.push_back(new Num(Elem::NUM, decl->get_dimensions().at(j), 0, false, 0));
            processed_access_exps.push_back(new Elem(Elem::MUL));
        }

        if (i != 0)
            processed_access_exps.push_back(new Elem(Elem::ADD));

    }

    print_exp(processed_access_exps);
    return processed_access_exps;
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

        if (e->get_elem_type() == syntax::Elem::NUM) {
            postfix.push_back(e);
        }
        else if (e->get_elem_type() == syntax::Elem::VAR) {
            Var* v = dynamic_cast<Var*>(e);
            if (symb_table.select_variable(v) == 0)
                throw semantic_exception(v->get_position(), string("Variável '" + v->get_identifier() + "' não declarada"));

            if (v->is_array())
                stack.push_back(v);
            else
                postfix.push_back(e);
        }
        else if (e->get_elem_type() == Elem::FUN) {
            Call* c = dynamic_cast<Call*>(e);
            if (!symb_table.select_function(c))
                throw semantic_exception(c->get_position(), "Função '" + c->get_identifier() + "' não declarada");

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
            if (stack.back()->get_elem_type() == syntax::Elem::FUN
                || stack.back()->get_elem_type() == syntax::Elem::VAR // Variáveis indexadas
                ) {
                postfix.push_back(stack.back());
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
    print_exp(postfix);
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