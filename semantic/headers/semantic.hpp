#ifndef SEMANTIC_HPP
#define SEMANTIC_HPP

#include <vector>

#include "syntax.hpp"

namespace semantic {

class SymbolTable {
    public:
    SymbolTable()
    {}

    int add_variable(syntax::Var* v) {
        for (auto var : variables) {
            if (var->get_identifier() == v->get_identifier())
                return var->get_index();
        }
        variables.push_back(v);
        return variables.size();
    }
    
    private:
    std::vector<syntax::Var*> variables;
};

} // namespace semantic

#endif // SEMANTIC_HPP