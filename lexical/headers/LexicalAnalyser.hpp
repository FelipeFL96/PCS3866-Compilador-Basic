#ifndef LEXICAL_ANALYZER_HPP
#define LEXICAL_ANALYZER_HPP

#include <fstream>
#include <string>
#include <exception>

#include "lexical.hpp"

#include "ASCIIClassifier.hpp"

namespace lexical {

class LexicalAnalyser {
    public:
        LexicalAnalyser(std::ifstream& file);
        lexical::token get_next();

    private:
        void change_analyser_state(lexical::token_type type);
        lexical::token extract_token();
        lexical::token read_comment();
        lexical::token_type categorize_token(std::string& value);

        std::ifstream& file;
        ASCIIClassifier ac;
        lexical::state analyser_state;
};

} // namespace lexical

#endif // LEXICAL_ANALYZER_HPP