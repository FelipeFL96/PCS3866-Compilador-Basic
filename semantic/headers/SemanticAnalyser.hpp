#ifndef SEMANTIC_ANALYSER_HPP
#define SEMANTIC_ANALYSER_HPP

#include <fstream>
#include <vector>

#include "SyntacticalAnalyser.hpp"

class SemanticAnalyser {
    public:
    SemanticAnalyser(std::ifstream& file);
    
    void parse_expression();

    private:
    void gen_exp_vector(syntax::Exp* e, std::vector<syntax::Elem*>& exp);
    void gen_negative_exp_vector(syntax::Exp* e, std::vector<syntax::Elem*>& exp);
    void convert_to_postfix(std::vector<syntax::Elem*>& exp);

    //std::ifstream& file;
    syntax::SyntacticalAnalyser stx;
};

#endif // SEMANTIC_ANALYSER_HPP