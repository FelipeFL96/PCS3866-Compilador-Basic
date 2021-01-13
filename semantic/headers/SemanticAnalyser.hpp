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
    std::vector<syntax::Elem*> gen_exp_vector(const std::vector<syntax::Eb*>& operands, const std::vector<syntax::Operator*>& operators);
    void convert_to_postfix(std::vector<syntax::Elem*>& exp);

    //std::ifstream& file;
    syntax::SyntacticalAnalyser stx;
};

#endif // SEMANTIC_ANALYSER_HPP