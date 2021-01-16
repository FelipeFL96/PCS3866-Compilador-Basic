#include <iostream>

#include "CodeGenerator.hpp"
#include "syntax.hpp"

using namespace std;

CodeGenerator::CodeGenerator(std::ofstream& file):
    file(file)
{}

void CodeGenerator::generate_header() {
    file << "/* GENERATED BY BASICC COMPILER */" << endl;
    file << ".global main" << endl;
    file << endl;
    /**/
    file << "func:" << endl;
    file << "LDMFD sp!, {r1}" << endl;
    file << "MUL r2, r1, r1" << endl;
    file << "MOV r3, #2" << endl;
    file << "MUL r0, r1, r3" << endl;
    file << "ADD r0, r0, r2" << endl;
    file << "ADD r0, r0, #1" << endl;
    file << "MOV pc, lr" << endl;
    file << endl;
    /**/
    file << "main: " << endl;
    /**/
    file << "LDR r12, =variables" << endl;
    file << "MOV r0, #2" << endl;
    file << "STR r0, [r12, #0]" << endl;
    file << "MOV r0, #5" << endl;
    file << "STR r0, [r12, #4]" << endl;
    /**/
    file << endl;
}

void CodeGenerator::generate_variables() {
    install_predef();
    file << "variables: .space 500" << endl;
    file << "stack: .space 500" << endl;
    file << endl;
}

void CodeGenerator::generate(syntax::Assign& assign) {
    file << "L" << assign.get_index() << ":" << endl;
    file << "\tLDR r0, =variables" << endl;
    file << "\tMOV r1, #" << assign.get_value() << endl;
    file << "\tSTR r1, [r0, #0]" << endl;
    file << endl;
}

void CodeGenerator::generate(syntax::Read& read) {
    file << "L" << read.get_index() << ":" << endl;
    file << endl;
}

void CodeGenerator::generate(syntax::Data& data) {
    file << "L" << data.get_index() << ":" << endl;
    file << endl;
}

void CodeGenerator::generate(syntax::Goto& go) {
    file << "L" << go.get_index() << ":" << endl;
    file << "\tB L" << go.get_destination() << endl;
    file << endl;
}

void CodeGenerator::generate_expression(vector<syntax::Elem*>& exp) {
    file << "\tLDR sp,    =stack" << endl;
    for (auto e : exp) {
        if (e->get_elem_type() == syntax::Elem::NUM) {
            file << "\tMOV      r1, #" << dynamic_cast<syntax::Num*>(e)->get_value() << endl;
            file << "\tSTMFD    sp!, {r1}" << endl;
        }
        else if (e->get_elem_type() == syntax::Elem::VAR) {
            file << "\tLDR      r1, [r12, #" << 4*dynamic_cast<syntax::Var*>(e)->get_index() << "]" << endl;
            file << "\tSTMFD    sp!, {r1}" << endl;
        }
        else if (e->get_elem_type() == syntax::Elem::FUN) {
            file << "\tBL       " << dynamic_cast<syntax::Call*>(e)->get_identifier() << endl;
            file << "\tSTMFD    sp!, {r0}" << endl;
        }
        else if (e->is_operator()) {
            file << "\tLDMFD    sp!, {r2}" << endl;
            file << "\tLDMFD    sp!, {r1}" << endl;
            if (e->get_elem_type() == syntax::Elem::ADD)
                file << "\tADD      r0, r1, r2" << endl;
            else if (e->get_elem_type() == syntax::Elem::SUB)
                file << "\tSUB      r0, r1, r2" << endl;
            else if (e->get_elem_type() == syntax::Elem::MUL)
                file << "\tMUL      r0, r1, r2" << endl;
            else if (e->get_elem_type() == syntax::Elem::DIV)
                file << "\tBL       sdiv" << endl;
            else if (e->get_elem_type() == syntax::Elem::POW)
                file << "\tBL       pow" << endl;
            file << "\tSTMFD    sp!, {r0}" << endl;
        }
    }
    file << "\tLDMFD    sp!, {r0}" << endl;
    file << endl;
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
    file << "sdiv:" << endl;
    file << "\tMOV      r4, #0" << endl;
    file << "\tMOV      r5, #0" << endl;
    file << endl;
    file << "\tCMP      r2, #0" << endl;
    file << "\tBEQ      sdiv.end" << endl;
    file << "\tMOVLT    r4, #1" << endl;
    file << "\tRSBLT    r2, r2, #0" << endl;
    file << endl;
    file << "\tCMP      r1, #0" << endl;
    file << "\tMOVLT    r5, #1" << endl;
    file << "\tRSBLT    r1, r1, #0" << endl;
    file << "\tEOR      r4, r4, r5" << endl;
    file << endl;
    file << "\tMOV      r0, #0" << endl;
    file << "\tMOV      r3, #1" << endl;
    file << endl;
    file << "sdiv.start:" << endl;
    file << "\tCMP      r2, r1" << endl;
    file << "\tMOVLS    r2, r2, LSL #1" << endl;
    file << "\tMOVLS    r3, r3, LSL #1" << endl;
    file << "\tBLS      sdiv.start" << endl;
    file << endl;
    file << "sdiv.next:" << endl;
    file << "\tCMP       r1, r2" << endl;
    file << "\tSUBCS     r1, r1, r2" << endl;
    file << "\tADDCS     r0, r0, r3" << endl;
    file << "\tMOVS      r3, r3, LSR #1" << endl;
    file << "\tMOVCC     r2, r2, LSR #1" << endl;
    file << "\tBCC       sdiv.next" << endl;
    file << endl;
    file << "sdiv.end:" << endl;
    file << "\tCMP      r4, #1" << endl;
    file << "\tRSBEQ    r0, r0, #0" << endl;
    file << "\tMOV      pc, lr" << endl;
    file << endl;
}

/*
 * POW r0, r1, r2
 * r0 = r1 ^ r2
 * Potenciação simples de inteiros
 * r0 = 0 se r2 < 0
 */
void CodeGenerator::install_pow() {
    file << "pow:" << endl;
    file << "\tCMP      r1, #0" << endl;
    file << "\tMOVEQ    r0, #0" << endl;
    file << "\tBEQ      pow.end" << endl;
    file << "\t" << endl;
    file << "\tCMP      r2, #0" << endl;
    file << "\tMOVLT    r0, #0" << endl;
    file << "\tBEQ      pow.end" << endl;
    file << "\t" << endl;
    file << "\tMOV      r0, #1" << endl;
    file << "\tMOV      r3, r2" << endl;
    file << endl;
    file << "pow.loop:" << endl;
    file << "\tCMP      r3, #0" << endl;
    file << "\tBEQ      pow.end" << endl;
    file << "\tMOV      r4, r0" << endl;
    file << "\tMUL      r0, r4, r1" << endl;
    file << "\tSUB      r3, r3, #1" << endl;
    file << "\tB        pow.loop" << endl;
    file << endl;
    file << "pow.end:" << endl;
    file << "\tMOV      pc, lr" << endl;
    file << endl;
}
