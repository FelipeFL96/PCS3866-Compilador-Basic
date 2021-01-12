#ifndef CODE_GENERATOR_HPP
#define CODE_GENERATOR_HPP

#include <fstream>

#include "syntax.hpp"

class CodeGenerator : public syntax::Generator {
    public:
        CodeGenerator(std::ofstream& file);

        void generate(syntax::Assign& assign);
        void generate(syntax::Read& read);
        void generate(syntax::Data& data);
        void generate(syntax::Goto& go);

        void generate_header();
        void generate_variables();

    private:
        std::ofstream& file;
};

#endif // CODE_GENERATOR_HPP