#ifndef SEMANTIC_ANALYSER_HPP
#define SEMANTIC_ANALYSER_HPP

#include <fstream>
#include <vector>

#include "SyntacticalAnalyser.hpp"
#include "CodeGenerator.hpp"

class SemanticAnalyser {
    public:
    SemanticAnalyser(std::ifstream& input, std::ofstream& output);
    
    void get_next(void);
    std::vector<syntax::Elem*> parse_expression();

    private:
    void gen_exp_vector(syntax::Exp* e, std::vector<syntax::Elem*>& exp);
    void gen_exp_vector_operand(syntax::Eb* operand, std::vector<syntax::Elem*>& exp);
    void gen_negative_exp_vector(syntax::Exp* e, std::vector<syntax::Elem*>& exp);
    std::vector<syntax::Elem*> convert_to_postfix(std::vector<syntax::Elem*>& exp);

    //std::ifstream& file;
    syntax::SyntacticalAnalyser stx;
    CodeGenerator gen;
};

#endif // SEMANTIC_ANALYSER_HPP