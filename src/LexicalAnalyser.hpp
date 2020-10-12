#ifndef LEXICAL_ANALYZER_HPP
#define LEXICAL_ANALYZER_HPP

#include <fstream>
#include <string>
#include <exception>

#include "lexical.hpp"

#include "ASCIIClassifier.hpp"

class LexicalAnalyser {
    public:
        LexicalAnalyser(std::ifstream& file);
        token get_next();    

    private:
        token extract_token();
        token_type categorize_token(std::string& value);

        std::ifstream& file;
        ASCIIClassifier ac;
};

#endif // LEXICAL_ANALYZER_HPP