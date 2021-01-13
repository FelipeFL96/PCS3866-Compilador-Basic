#ifndef SEMANTIC_ANALYSER_HPP
#define SEMANTIC_ANALYSER_HPP

#include <fstream>

#include "SyntacticalAnalyser.hpp"

class SemanticAnalyser {
    public:
    SemanticAnalyser(std::ifstream& file);
    
    void parse_expression();

    private:
    //std::ifstream& file;
    syntax::SyntacticalAnalyser stx;
};

#endif // SEMANTIC_ANALYSER_HPP