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
        virtual void generate(class Assign* assign) = 0;
        virtual void generate(class Read* go) = 0;
        virtual void generate(class Data* go) = 0;
        virtual void generate(class Goto* go) = 0;
};

class Elem {
    public:
        enum type {
            NUM,
            VAR,
            FUN,
            ADD,
            SUB,
            MUL,
            DIV,
            POW,
            PRO,
            PRC,
            EXP,
            COM
        };

        Elem(Elem::type type):
            type_(type)
        {}

        virtual Elem::type get_elem_type() const {
            return type_;
        }

        bool is_operator() {
            return (type_ == Elem::ADD) 
                || (type_ == Elem::SUB)
                || (type_ == Elem::MUL)
                || (type_ == Elem::DIV)
                || (type_ == Elem::POW);
        }

    private:
        Elem::type type_;

};

class Operator : public Elem {
    public:
        enum type {
            ADD,
            SUB,
            MUL,
            DIV,
            POW
        };

        Operator(Elem::type elem, Operator::type type, std::string symbol):
            Elem(elem), type_(type), symbol_(symbol)
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

class Eb : public Elem {
    public:
        enum type {
            NUM,
            VAR,
            CALL,
            EXP
        };

        Eb(Elem::type elem):
            Elem(elem)
        {}

        virtual Eb::type get_eb_type() = 0;
};

class Num : public Eb {
    public:
        Num(Elem::type elem, int integer, bool neg_exp, int exponent):
            Eb(elem), integer_(integer), neg_exp_(neg_exp), exponent_(exponent)
        {}

        Num(bool negative, Num* n):
            Eb(Elem::NUM), negative_(negative), integer_(n->integer_), neg_exp_(n->neg_exp_), exponent_(n->exponent_)
        {
            delete n;
        }

        bool get_negative() {
            return negative_;
        }

        int get_integer() {
            return integer_;
        }

        bool get_neg_exp() {
            return neg_exp_;
        }

        int get_exponent() {
            return exponent_;
        }

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
            return (negative_?  (-num) : num);
        }

        Eb::type get_eb_type() {
            return Eb::NUM;
        }

    private:
        bool negative_ = false;
        int integer_ = 0;
        bool neg_exp_ = false;
        int exponent_ = 0;
};

class Var : public Eb {
    public:
        Var(Elem::type elem, std::string identifier):
            Eb(elem), identifier_(identifier)
        {}

        std::string get_identifier() {
            return identifier_;
        }

        int get_index() {
            return index_;
        }

        void set_index(int index) {
            index_ = index;
        }

        Eb::type get_eb_type() {
            return Eb::VAR;
        }

    private:
        std::string identifier_;
        bool indexed;
        int dimension;

        int index_, size;
};

class Exp : public Eb {
    public:
        Exp(Elem::type elem, bool negative, std::vector<Eb*> operands, std::vector<Operator*> operators):
            Eb(elem), negative_(negative), operands_(operands), operators_(operators)
        {}

    void make_positive() {
        negative_ = false;
    }

    bool is_negative() {
        return negative_;
    }

    std::vector<Eb*> get_operands() {
        return operands_;
    }

    std::vector<Operator*> get_operators() {
        return operators_;
    }

    Eb::type get_eb_type() {
        return Eb::EXP;
    }

    private:
        bool negative_;
        std::vector<Eb*> operands_;
        std::vector<Operator*> operators_;
};

class Call : public Eb {
    public:
        Call(Elem::type elem, std::string identifier, std::vector<Exp*>& args):
            Eb(elem), identifier_(identifier), args_(args)
        {}

        std::string get_identifier() {
            return identifier_;
        }

        std::vector<Exp*> get_args() {
            return args_;
        }

        // Provisório: o objeto chamada precisa ser ligado à declaração correspondente
        // A declaração de função é quem deve dizer a quantidade de argumentos esperada
        int param_count() {
            return args_.size();
        }

        Eb::type get_eb_type() {
            return Eb::CALL;
        }

    private:
        std::string identifier_;
        std::vector<Exp*> args_;
};

class BStatement {
    public:
        BStatement(int index, lexic::position pos):
            index_(index), pos_(pos)
        {}

        virtual int get_index() {
            return index_;
        }

        virtual lexic::position& get_position() {
            return pos_;
        }

    private:
        int index_;
        lexic::position pos_;
};

class Assign : public BStatement {
    public:
        Assign(int index, lexic::position pos, Var* variable, Exp* expression):
            BStatement(index, pos), variable_(variable), expression_(expression)
        {}

        Var* get_variable() {
            return variable_;
        }

        Exp* get_expression() {
            return expression_;
        }

        void accept(Generator& gen) {
            gen.generate(this);
        }

    private:
        Var* variable_;
        Exp* expression_;
};

class Read : public BStatement {
    public:
        Read(int index, lexic::position pos, std::vector<Var*> variables):
            BStatement(index, pos), variables_(variables)
        {}

        std::vector<Var*> get_variables() {
            return variables_;
        }

        void accept(Generator& gen) {
            gen.generate(this);
        }

    private:
        std::vector<Var*> variables_;
};

class Data : public BStatement {
    public:
        Data(int index, lexic::position pos, std::vector<Num*> values):
            BStatement(index, pos), values_(values)
        {}

        std::vector<Num*> get_values() {
            return values_;
        }

        void accept(Generator& gen) {
            gen.generate(this);
        }

    private:
        std::vector<Num*> values_;
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
        Goto(int index, lexic::position pos, int destination):
            BStatement(index, pos), destination_(destination)
        {}

        int get_destination() {
            return destination_;
        }

        void accept(Generator& gen) {
            gen.generate(this);
        }

    private:
        int destination_;
};

class If : public BStatement {
    public:
        enum cmp {
            EQL, NEQ, GTN, LTN, GEQ, LEQ
        };

        If(int index, lexic::position pos, Exp* left, If::cmp op, Exp* right, int destination):
            BStatement(index, pos), left_(left), right_(right), op_(op), destination_(destination)
        {}

        Exp* get_left() {
            return left_;
        }

        Exp* get_right() {
            return right_;
        }

        If::cmp get_op() {
            return op_;
        }

        int get_destination() {
            return destination_;
        }

    private:
        Exp *left_, *right_;
        If::cmp op_;
        int destination_;
};

class For : public BStatement  {
    public:
        For(int index, lexic::position pos, Var* iterator, Exp* init, Exp* stop, Exp* step):
            BStatement(index, pos), iterator_(iterator), init_(init), stop_(stop), step_(step)
        {}

        Var* get_iterator() {
            return iterator_;
        }

        Exp* get_init() {
            return init_;
        }

        Exp* get_stop() {
            return stop_;
        }

        Exp* get_step() {
            return step_;
        }

        int get_next_line() {
            return next_line_;
        }

        void set_next_line(int next_line) {
            next_line_ = next_line;
        }

    private:
        Var* iterator_;
        Exp *init_, *stop_, *step_;
        int next_line_;
};

class Next : public BStatement {
    public:
        Next(int index, lexic::position pos, Var* iterator):
            BStatement(index, pos), iterator_(iterator)
        {}

        Var* get_iterator() {
            return iterator_;
        }

        For* get_loop() {
            return loop_;
        }

        void set_loop(For* loop) {
            loop_ = loop;
        }

    private:
        Var* iterator_;
        For* loop_;
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