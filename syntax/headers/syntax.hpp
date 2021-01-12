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
        virtual void generate(class Read& go) = 0;
        virtual void generate(class Data& go) = 0;
        virtual void generate(class Goto& go) = 0;
};

class Elem {
    public:
        enum type {
            NUM,
            VAR,
            ADD,
            SUB,
            MUL,
            DIV,
            PRO,
            PRC
        };

        Elem(Elem::type type):
            type_(type)
        {}

    private:
        Elem::type type_;

};

class Operator {
    public:
        enum type {
            ADD,
            SUB,
            MUL,
            DIV,
            POW
        };

        Operator(Operator::type type, std::string symbol):
            type_(type), symbol_(symbol)
        {}

        Operator::type get_op_type() {
            return type_;
        }

        std::string get_symbol() {
            return symbol_;
        }

    private:
        Operator::type type_;
        std::string symbol_;
};

class Eb {
    public:
        enum type {
            NUM,
            VAR,
            CALL,
            EXP
        };

        virtual Eb::type get_eb_type() = 0;
};

class Num : public Eb {
    public:
        Num(int integer, bool neg_exp, int exponent):
            integer_(integer), neg_exp_(neg_exp), exponent_(exponent)
        {}

        int get_value() {
            int num = integer_;

            if (neg_exp_) {
                for (int i = 0; i < exponent_; i++) {
                    num /= 10;
                }
            }
            else {
                for (int i = 0; i < exponent_; i++) {
                    num *= 10;
                }
            }
            return num;
        }

        Eb::type get_eb_type() {
            return Eb::NUM;
        }

    private:
        int integer_ = 0;
        bool neg_exp_ = false;
        int exponent_ = 0;
};

class Var : public Eb {
    public:
        Var(std::string identifier):
            identifier_(identifier)
        {}

        Eb::type get_eb_type() {
            return Eb::VAR;
        }

    private:
        std::string identifier_;
        bool indexed;
        int dimension;

        int index, size;
};

class Exp {
    public:
        Exp(bool positive, std::vector<Eb*> operands, std::vector<Operator*> operators):
            positive_(positive), operands_(operands), operators_(operators)
        {}

    bool is_positive() {
        return positive_;
    }

    std::vector<Eb*> get_operands() {
        return operands_;
    }

    std::vector<Operator*> get_operators() {
        return operators_;
    }

    private:
        bool positive_;
        std::vector<Eb*> operands_;
        std::vector<Operator*> operators_;
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

        void accept(Generator& gen) {
            gen.generate(*this);
        }

    private:
        std::vector<std::string> identifiers_;
        //std::vector<Var> variables;
};

class Data : public BStatement {
    public:
        Data(int index, std::vector<int>& values):
            BStatement(index), values_(values)
        {}

        std::vector<int> get_values() {
            return values_;
        }

        void accept(Generator& gen) {
            gen.generate(*this);
        }

    private:
        std::vector<int> values_;
        //std::vector<Num> values;
};

class Pitem {
    enum class type {
        string,
        variable,
        number
    };

    Pitem::type type;

    Num num;
    Var var;
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
        syntax_exception(lexic::position& loc, const std::string error_message)
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

} // namespace syntax

#endif // SYNTAX_HPP