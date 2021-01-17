#ifndef CODE_GENERATOR_HPP
#define CODE_GENERATOR_HPP

#include <fstream>
#include <vector>
#include <utility>

#include "syntax.hpp"
#include "semantic.hpp"

class CodeGenerator/* : public syntax::Generator*/ {
    public:
        CodeGenerator(std::ifstream& input, std::ofstream& output, semantic::SymbolTable& symb_table);

        void generate(syntax::Assign* assign, std::vector<syntax::Elem*> exp);
        void generate(syntax::Read* read, std::vector<std::pair<syntax::Var*, syntax::Num*>>& read_data);
        void generate(syntax::Data* data, std::vector<std::pair<syntax::Var*, syntax::Num*>>& read_data);
        void generate(syntax::Goto* go);
        void generate(syntax::If* ift, std::vector<syntax::Elem*> left, std::vector<syntax::Elem*> right);
        void generate(syntax::For* loop, std::vector<syntax::Elem*> init, std::vector<syntax::Elem*> stop, std::vector<syntax::Elem*> step);

        void generate_expression(std::vector<syntax::Elem*>& exp);
        void generate_header();
        void generate_variables();

        void install_predef();
        void install_sdiv();
        void install_pow();

    private:
        std::ifstream& input;
        std::ofstream& output;
        semantic::SymbolTable& symb_table;

};

#endif // CODE_GENERATOR_HPP