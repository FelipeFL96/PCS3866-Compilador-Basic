#ifndef SYNTAX_HPP
#define SYNTAX_HPP

#include <string>
#include <vector>

#include "lexic.hpp"

namespace syntax {

enum class type {
    ASSIGN,         // Atribuição de variáveis
    READ,
    DATA,
    GOTO,           // Desvio incondicional
    IF              // Desvio condicional
};

class Generator {
    public:
        virtual void generate(class Assign& assign) = 0;
        virtual void generate(class Goto& go) = 0;
};

class Number {
public:
    bool negative = 0;
    int integer = 0;
    int fraction = 0;
    bool exponent_negative = 0;
    int exponent = 0;

    double generate() {
        double num = 0.0;
        num = integer;

        double frac = fraction;
        while (frac > 1) {
            frac /= 10;
        }
        num = frac + num;

        if (negative)
            num *= -1;

        if (exponent_negative) {
            for (int i = 0; i < exponent; i++) {
                num /= 10;
            }
        }
        else {
            for (int i = 0; i < exponent; i++) {
                num *= 10;
            }
        }
        return num;
    }
};


class Eb;

class Exp {};

class Variable {
    std::string code;
    std::string identifier;
};

class Syntaxeme {
    public:
    virtual void accept(Generator& gen) = 0;
};

class BStatement : public Syntaxeme {
    public:
        BStatement(int index):
            index_(index)
        {}

        int get_index() {
            return index_;
        }

    private:
        int index_;
};

class Assign : public BStatement {
    public:
        Assign(int index, std::string identifier, int value):
            BStatement(index), identifier_(identifier), value_(value)
        {}

        std::string get_identifier() {
            return identifier_;
        }

        int get_value() {
            return value_;
        }

        void accept(Generator& gen) {
            gen.generate(*this);
        }

    private:
        std::string identifier_;
        int value_;
    //Exp expr;
};

class Read : public BStatement {
    public:
        Read(int index, std::vector<std::string>& identifiers):
            BStatement(index), identifiers_(identifiers)
        {}

        std::vector<std::string> get_identifiers() {
            return identifiers_;
        }

    private:
        std::vector<std::string> identifiers_;
        //std::vector<Variable> variables;
};

class Data : public BStatement {
    public:
        Data(int index, std::vector<int>& values):
            BStatement(index), values_(values)
        {}

        std::vector<int> get_values() {
            return values_;
        }

    private:
        std::vector<int> values_;
        //std::vector<Number> values;
};

class Pitem {
    enum class type {
        string,
        variable,
        number
    };

    Pitem::type type;

    Number num;
    Variable var;
    std::string str;
};

class Print {
    std::vector<Pitem> pitems;
};

class Goto : public BStatement {
    public:
        Goto(int index, int destination):
            BStatement(index), destination_(destination)
        {}

        int get_destination() {
            return destination_;
        }

        void accept(Generator& gen) {
            gen.generate(*this);
        }

    private:
        int destination_;
};



class If {
    Exp left;
    Exp right;
    lexic::type op;
    int destination;
};

class syntax_exception: public std::exception {
    public:
        syntax_exception(lexic::position& loc, const char* error_message)
            : loc(loc), error_message(error_message) {
            exception();
        }

        std::string message() {
            return loc.position_str() + ": " + error_message;
        }

    private:
        lexic::position& loc;
        const char* error_message;
};

} // namespace syntax

#endif // SYNTAX_HPP