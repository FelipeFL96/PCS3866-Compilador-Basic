#ifndef SEMANTIC_ANALYSER_HPP
#define SEMANTIC_ANALYSER_HPP

#include <fstream>
#include <vector>
#include <queue>

#include "syntax.hpp"
#include "semantic.hpp"
#include "SyntacticalAnalyser.hpp"
#include "CodeGenerator.hpp"

namespace semantic {

class SemanticAnalyser {
    public:
    SemanticAnalyser(std::ifstream& input, std::ofstream& output);
    
    void get_next(void);
    std::vector<syntax::Elem*> parse_expression(syntax::Exp* e);
    void treat_variable(syntax::Var* v);

    private:
    void gen_exp_vector(syntax::Exp* e, std::vector<syntax::Elem*>& exp);
    void gen_exp_vector_operand(syntax::Eb* operand, std::vector<syntax::Elem*>& exp);
    void gen_negative_exp_vector(syntax::Exp* e, std::vector<syntax::Elem*>& exp);
    std::vector<syntax::Elem*> convert_to_postfix(std::vector<syntax::Elem*>& exp);

    syntax::SyntacticalAnalyser stx;
    CodeGenerator gen;
    semantic::SymbolTable symb_table;
    std::queue<syntax::Var*> read_variables;
};

} // namespace semantic

#endif // SEMANTIC_ANALYSER_HPP