#include <iostream>

#include "syntax.hpp"
#include "CodeGenerator.hpp"

#define STACK_SIZE 256

using namespace std;

CodeGenerator::CodeGenerator(ifstream& input, ofstream& output, semantic::SymbolTable& symb_table):
    input(input), output(output), symb_table(symb_table)
{}

void CodeGenerator::generate_header() {
    output << "/* BASIC COMPILER */" << endl;
    output << ".global main" << endl;
    output << endl;
    output << "main: " << endl;
    output << "\tLDR      r12, =variables" << endl;
    output << "\tLDR      sp, =stack" << endl;
    /*/
    output << "func:" << endl;
    output << "LDMFD sp!, {r1}" << endl;
    output << "MUL r2, r1, r1" << endl;
    output << "MOV r3, #2" << endl;
    output << "MUL r0, r1, r3" << endl;
    output << "ADD r0, r0, r2" << endl;
    output << "ADD r0, r0, #1" << endl;
    output << "MOV pc, lr" << endl;
    output << endl;
    /*/
    /*/
    output << "LDR r12, =variables" << endl;
    output << "MOV r0, #2" << endl;
    output << "STR r0, [r12, #0]" << endl;
    output << "MOV r0, #5" << endl;
    output << "STR r0, [r12, #4]" << endl;
    /*/
    output << endl;
}

void CodeGenerator::generate_variables() {
    install_predef();
    output << "variables: " << endl;
    output << "\t.space " << symb_table.total_variable_size() << endl;
    output << endl;
    output << "stack: " << endl;
    output << "\t.space " << STACK_SIZE << endl;
    output << endl;
}

void CodeGenerator::generate(syntax::Assign* assign, vector<syntax::Elem*> exp, int next_index) {
    output << "L" << assign->get_index() << ":" << endl;
    generate_expression(exp);
    output << "\tSTR      r0, [r12, #" << 4 * symb_table.select_variable(assign->get_variable()) << "]" << endl;
    output << "\tB        L" << next_index << endl;
    output << endl;
}

void CodeGenerator::generate(syntax::Read* read, std::vector<pair<syntax::Var*, syntax::Num*>>& read_data, int next_index) {
    output << "L" << read->get_index() << ":" << endl;
    for (auto pair : read_data) {
        output << "\tMOV      r0, #" << get<1>(pair)->get_value() << endl;
        output << "\tSTR      r0, [r12, #" << 4 * symb_table.select_variable(get<0>(pair)) << "]" << endl;
    }
    output << "\tB        L" << next_index << endl;
    output << endl;
}

void CodeGenerator::generate(syntax::Data* data, std::vector<pair<syntax::Var*, syntax::Num*>>& read_data, int next_index) {
    output << "L" << data->get_index() << ":" << endl;
    for (auto pair : read_data) {
        output << "\tMOV      r0, #" << get<1>(pair)->get_value() << endl;
        output << "\tSTR      r0, [r12, #" << 4 * symb_table.select_variable(get<0>(pair)) << "]" << endl;
    }
    output << "\tB        L" << next_index << endl;
    output << endl;
}

void CodeGenerator::generate(syntax::Goto* go) {
    output << "L" << go->get_index() << ":" << endl;
    output << "\tB        L" << go->get_destination() << endl;
    output << endl;
}

void CodeGenerator::generate(syntax::If* ift, std::vector<syntax::Elem*> left, std::vector<syntax::Elem*> right, int next_index) {
    output << "L" << ift->get_index() << ":" << endl;
    generate_expression(left);
    output << "\tSTMFD    sp!, {r0}" << endl;
    generate_expression(right);
    output << "\tLDMFD    sp!, {r1}" << endl;
    output << "\tCMP      r1, r0" << endl;
    switch (ift->get_op()) {
        case syntax::If::EQL:
            output << "\tBEQ      " << "L" << ift->get_destination() << endl;
            break;
        case syntax::If::NEQ:
            output << "\tBNE      " << "L" << ift->get_destination() << endl;
            break;
        case syntax::If::GTN:
            output << "\tBGT      " << "L" << ift->get_destination() << endl;
            break;
        case syntax::If::LTN:
            output << "\tBLT      " << "L" << ift->get_destination() << endl;
            break;
        case syntax::If::GEQ:
            output << "\tBGE      " << "L" << ift->get_destination() << endl;
            break;
        case syntax::If::LEQ:
            output << "\tBLE      " << "L" << ift->get_destination() << endl;
            break;
    }
    output << "\tB        L" << next_index << endl;
    output << endl;
}

void CodeGenerator::generate(syntax::For* loop, vector<syntax::Elem*> init, vector<syntax::Elem*> stop,
            vector<syntax::Elem*> step, int index_inside_loop, int index_outside_loop) {
    output << "L" << loop->get_index() << ":" << endl;

    // Inicialização do iterador
    generate_expression(init);
    output << "\tSTR      r0, [r12, #" << 4 * symb_table.select_variable(loop->get_iterator()) << "]" << endl;
    output << "\tB        L" << loop->get_index() << ".COMP" << endl;
    output << endl;

    // Incremento do iterador
    output << "L" << loop->get_index() << ".STEP:" << endl;
    generate_expression(step);
    output << "\tLDR      r1, [r12, #" << 4 * symb_table.select_variable(loop->get_iterator()) << "]" << endl;
    output << "\tADD      r0, r1, r0" << endl;
    output << "\tSTR      r0, [r12, #" << 4 * symb_table.select_variable(loop->get_iterator()) << "]" << endl;
    output << endl;

    // Comparação com a condição de parada
    output << "L" << loop->get_index() << ".COMP:" << endl;
    generate_expression(stop);
    output << "\tLDR      r1, [r12, #" << 4 * symb_table.select_variable(loop->get_iterator()) << "]" << endl;
    output << "\tCMP      r1, r0" << endl;
    output << "\tBGE      L" << index_outside_loop << endl;
    output << "\tB        L" << index_inside_loop << endl;
    output << endl;
}

void CodeGenerator::generate(syntax::Next* next) {
    output << "L" << next->get_index() << ":" << endl;
    output << "\tB        L" << next->get_loop()->get_index() << ".STEP" << endl;
    output << endl;
}

void CodeGenerator::generate(syntax::Def* def, std::vector<syntax::Elem*>& exp) {
    output << def->get_identifier() << ":" << endl;

    auto parameters = def->get_parameters();
    while (!parameters.empty()) {
        auto param = parameters.back();
        output << "\tLDMFD    sp!, {r1}" << endl;
        output << "\tSTR      [r12, #"
            << 4 * symb_table.select_variable(param) << "]" << endl;
        parameters.pop_back();
    }

    generate_expression(exp);
    output << "\tMOV      pc, lr" << endl;
    output << endl;
}

void CodeGenerator::generate(syntax::Gosub* gosub) {
    output << "L" << gosub->get_index() << ":" << endl;
    output << "\tBL       L" << gosub->get_destination() << endl;
    output << endl;
}

void CodeGenerator::generate(syntax::Return* ret) {
    output << "L" << ret->get_index() << ":" << endl;
    output << "\tMOV      pc, lr" << endl;
    output << endl;
}

void CodeGenerator::generate(syntax::End* end) {
    output << "L" << end->get_index() << ":" << endl;
    output << "\tB        L" << end->get_index() << endl;
    output << endl;
}

void CodeGenerator::generate_expression(vector<syntax::Elem*>& exp) {
    for (auto e : exp) {
        if (e->get_elem_type() == syntax::Elem::NUM) {
            output << "\tMOV      r1, #" << dynamic_cast<syntax::Num*>(e)->get_value() << endl;
            output << "\tSTMFD    sp!, {r1}" << endl;
        }
        else if (e->get_elem_type() == syntax::Elem::VAR) {
            output << "\tLDR      r1, [r12, #" << 4 * symb_table.select_variable(dynamic_cast<syntax::Var*>(e)) << "]" << endl;
            output << "\tSTMFD    sp!, {r1}" << endl;
        }
        else if (e->get_elem_type() == syntax::Elem::FUN) {
            output << "\tBL       " << dynamic_cast<syntax::Call*>(e)->get_identifier() << endl;
            output << "\tSTMFD    sp!, {r0}" << endl;
        }
        else if (e->is_operator()) {
            output << "\tLDMFD    sp!, {r2}" << endl;
            output << "\tLDMFD    sp!, {r1}" << endl;
            if (e->get_elem_type() == syntax::Elem::ADD)
                output << "\tADD      r0, r1, r2" << endl;
            else if (e->get_elem_type() == syntax::Elem::SUB)
                output << "\tSUB      r0, r1, r2" << endl;
            else if (e->get_elem_type() == syntax::Elem::MUL)
                output << "\tMUL      r0, r1, r2" << endl;
            else if (e->get_elem_type() == syntax::Elem::DIV)
                output << "\tBL       sdiv" << endl;
            else if (e->get_elem_type() == syntax::Elem::POW)
                output << "\tBL       pow" << endl;
            output << "\tSTMFD    sp!, {r0}" << endl;
        }
    }
    output << "\tLDMFD    sp!, {r0}" << endl;
}

void CodeGenerator::install_predef() {
    install_sdiv();
    install_pow();
}

/*
 * SDIV r0, r1, r2
 * r0 = r1 / r2
 * Divisão inteira
 * r0 = 0 se r1 = 0
 */
void CodeGenerator::install_sdiv() {
    output << "sdiv:" << endl;
    output << "\tMOV      r4, #0" << endl;
    output << "\tMOV      r5, #0" << endl;
    output << endl;
    output << "\tCMP      r2, #0" << endl;
    output << "\tBEQ      sdiv.end" << endl;
    output << "\tMOVLT    r4, #1" << endl;
    output << "\tRSBLT    r2, r2, #0" << endl;
    output << endl;
    output << "\tCMP      r1, #0" << endl;
    output << "\tMOVLT    r5, #1" << endl;
    output << "\tRSBLT    r1, r1, #0" << endl;
    output << "\tEOR      r4, r4, r5" << endl;
    output << endl;
    output << "\tMOV      r0, #0" << endl;
    output << "\tMOV      r3, #1" << endl;
    output << endl;
    output << "sdiv.start:" << endl;
    output << "\tCMP      r2, r1" << endl;
    output << "\tMOVLS    r2, r2, LSL #1" << endl;
    output << "\tMOVLS    r3, r3, LSL #1" << endl;
    output << "\tBLS      sdiv.start" << endl;
    output << endl;
    output << "sdiv.next:" << endl;
    output << "\tCMP       r1, r2" << endl;
    output << "\tSUBCS     r1, r1, r2" << endl;
    output << "\tADDCS     r0, r0, r3" << endl;
    output << "\tMOVS      r3, r3, LSR #1" << endl;
    output << "\tMOVCC     r2, r2, LSR #1" << endl;
    output << "\tBCC       sdiv.next" << endl;
    output << endl;
    output << "sdiv.end:" << endl;
    output << "\tCMP      r4, #1" << endl;
    output << "\tRSBEQ    r0, r0, #0" << endl;
    output << "\tMOV      pc, lr" << endl;
    output << endl;
}

/*
 * POW r0, r1, r2
 * r0 = r1 ^ r2
 * Potenciação simples de inteiros
 * r0 = 0 se r2 < 0
 */
void CodeGenerator::install_pow() {
    output << "pow:" << endl;
    output << "\tCMP      r1, #0" << endl;
    output << "\tMOVEQ    r0, #0" << endl;
    output << "\tBEQ      pow.end" << endl;
    output << "\t" << endl;
    output << "\tCMP      r2, #0" << endl;
    output << "\tMOVLT    r0, #0" << endl;
    output << "\tBEQ      pow.end" << endl;
    output << "\t" << endl;
    output << "\tMOV      r0, #1" << endl;
    output << "\tMOV      r3, r2" << endl;
    output << endl;
    output << "pow.loop:" << endl;
    output << "\tCMP      r3, #0" << endl;
    output << "\tBEQ      pow.end" << endl;
    output << "\tMOV      r4, r0" << endl;
    output << "\tMUL      r0, r4, r1" << endl;
    output << "\tSUB      r3, r3, #1" << endl;
    output << "\tB        pow.loop" << endl;
    output << endl;
    output << "pow.end:" << endl;
    output << "\tMOV      pc, lr" << endl;
    output << endl;
}
