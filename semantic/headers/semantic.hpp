#ifndef SEMANTIC_HPP
#define SEMANTIC_HPP

#include <iostream>
#include <vector>

#include "syntax.hpp"

namespace semantic {

class SymbolTable {
    public:
    SymbolTable()
    {}

    int insert_variable(syntax::Var* v) {
        if (int index = select_variable(v) != 0)
            return index;

        variables.push_back(v);
        return variables.size();
    }

    int select_variable(syntax::Var* v) {
        //using namespace std;
        if (variables.empty())
            return 0;

        //cout << "Procurando por variável de nome: " << v->get_identifier() << endl;
        for (auto var : variables) {
            //cout << "Testando [" << var->get_identifier() << ": ";
            if (var->get_identifier() == v->get_identifier()) {
                //cout << "APROVADA" << endl;
                return var->get_index();
            }
            //cout << "REPROVADA" << endl;
        }

        return 0;
    }
    
    private:
    std::vector<syntax::Var*> variables;
};

} // namespace semantic

#endif // SEMANTIC_HPP