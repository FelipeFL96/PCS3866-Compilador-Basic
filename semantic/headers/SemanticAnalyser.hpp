#ifndef SEMANTIC_ANALYSER_HPP
#define SEMANTIC_ANALYSER_HPP

#include <fstream>
#include <utility>
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

    private:
    void process_assign(syntax::Assign* assign);
    void process_read(syntax::Read* read);
    void process_data(syntax::Data* data);
    void process_print(syntax::Print* print);
    void process_goto(syntax::Goto* go);
    void process_if(syntax::If* ift);
    void process_for(syntax::For* loop);
    void process_next(syntax::Next* next);
    void process_dim(syntax::Dim* dim);
    void process_def(syntax::Def* def);
    void process_gosub(syntax::Gosub* gosub);
    void process_return(syntax::Return* ret);
    void process_end(syntax::End* end);

    std::vector<syntax::Elem*> process_expression(syntax::Exp* e);
    void process_variable(syntax::Var* v);

    void gen_exp_vector(syntax::Exp* e, std::vector<syntax::Elem*>& exp);
    void gen_exp_vector_operand(syntax::Eb* operand, std::vector<syntax::Elem*>& exp);
    void gen_negative_exp_vector(syntax::Exp* e, std::vector<syntax::Elem*>& exp);
    std::vector<syntax::Elem*> convert_to_postfix(std::vector<syntax::Elem*>& exp);

    syntax::SyntacticalAnalyser stx;
    CodeGenerator gen;
    semantic::SymbolTable symb_table;
    std::queue<syntax::Var*> read_variables;
    std::queue<syntax::Num*> data_values;
    std::vector<syntax::For*> for_stack;
};

} // namespace semantic

#endif // SEMANTIC_ANALYSER_HPP