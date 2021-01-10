#include <iostream>

#include "CodeGenerator.hpp"
#include "syntax.hpp"

using namespace std;

CodeGenerator::CodeGenerator(std::ofstream& file):
    file(file)
{}

void CodeGenerator::generate_header() {
    file << "; GENERATED BY BASICC COMPILER" << endl;
    file << ".global START" << endl;
    file << endl;
    file << "START: " << endl;
    file << endl;
}

void CodeGenerator::generate_variables() {
    file << "VARIABLES: " << endl;
    file << endl;
}

void CodeGenerator::generate(syntax::Assign& assign) {
    file << "L" << assign.get_index() << ":" << endl;
    file << "\tLDR r0, =VARIABLES" << endl;
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