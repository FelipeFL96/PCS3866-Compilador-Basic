#ifndef CODE_GENERATOR_HPP
#define CODE_GENERATOR_HPP

#include <fstream>

#include "syntax.hpp"
#include "semantic.hpp"

class CodeGenerator/* : public syntax::Generator*/ {
    public:
        CodeGenerator(std::ofstream& file, semantic::SymbolTable& symb_table);

        void generate(syntax::Assign* assign, std::vector<syntax::Elem*> exp);
        void generate(syntax::Read* read);
        void generate(syntax::Data* data);
        void generate(syntax::Goto* go);

        void generate_expression(std::vector<syntax::Elem*>& exp);
        void generate_header();
        void generate_variables();

        void install_predef();
        void install_sdiv();
        void install_pow();

    private:
        std::ofstream& file;
        semantic::SymbolTable& symb_table;

};

#endif // CODE_GENERATOR_HPP