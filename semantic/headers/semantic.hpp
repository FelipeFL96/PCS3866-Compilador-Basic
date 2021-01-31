#ifndef SEMANTIC_HPP
#define SEMANTIC_HPP

#include <iostream>
#include <exception>
#include <vector>

#include "syntax.hpp"

namespace semantic {

class SymbolTable {
    public:
    SymbolTable()
    {}

    int insert_variable(syntax::Var* v) {
        if (int var_index = select_variable(v) != 0)
            return var_index;

        variables.push_back(v);
        return index++;
    }

    int insert_array(syntax::Array* a) {
        if (int var_index = select_variable(dynamic_cast<syntax::Var*>(a)) != 0)
            return var_index;

        variables.push_back(a);
        int var_index = index;

        int size = 1;
        for (int d : a->get_dimensions())
            size *= d;
        index += size;

        return var_index;
    }

    int select_variable(syntax::Var* v) {
        using namespace std;
        if (variables.empty())
            return 0;

        for (auto var : variables) {
            if (var->get_identifier() == v->get_identifier()) {
                return var->get_index();
            }
        }

        return 0;
    }

    syntax::Var* pointer_to_variable(syntax::Var* v) {
        if (variables.empty())
            return nullptr;

        for (auto var : variables) {
            if (var->get_identifier() == v->get_identifier()) {
                return var;
            }
        }

        return nullptr;
    }

    int total_variable_size() {
        int total_size = 4;
        for (auto var : variables) {
            total_size += var->get_size();
        }
        return total_size;
    }

    void print_variables() {
        for (int i = 0; i < variables.size(); i++) {
            std::cout << "[" << variables.at(i)->get_index() << "] " << variables.at(i)->get_identifier() << std::endl;
        }
    }

    bool insert_function(syntax::Def* f) {
         if (select_function(f))
            return false;

        functions.push_back(f);
        return true;
    }

    syntax::Def* select_function(syntax::Call* c) {
        if (functions.empty())
            return nullptr;

        for (auto fn : functions) {
            if (fn->get_identifier() == c->get_identifier())
                return fn;
        }

        return nullptr;
    }

    syntax::Def* select_function(syntax::Def* f) {
        if (functions.empty())
            return nullptr;

        for (auto fn : functions) {
            if (fn->get_identifier() == f->get_identifier())
                return fn;
        }

        return nullptr;
    }
    
    private:
    int index = 1;
    std::vector<syntax::Var*> variables;
    std::vector<syntax::Def*> functions;
};

class semantic_exception: public std::exception {
    public:
        semantic_exception(lexic::position& loc, const std::string error_message)
            : loc(loc), error_message(error_message) {
            exception();
        }

        std::string message() {
            return loc.position_str() + ": " + error_message;
        }

    private:
        lexic::position& loc;
        const std::string error_message;
};

} // namespace semantic

#endif // SEMANTIC_HPP