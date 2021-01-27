#ifndef SYNTAX_ANALYSER_HPP
#define SYNTAX_ANALYSER_HPP

#include <iostream>
#include <string>

#include "syntax.hpp"
#include "lexic.hpp"
#include "LexicalAnalyser.hpp"

namespace syntax {

class SyntaxAnalyser {
    public:
        SyntaxAnalyser(std::ifstream& file);

        syntax::BStatement* get_next(void);

    private:
        std::ifstream& file;
        lexic::LexicalAnalyser lex;
        lexic::token tk;
        bool token_consumed = true;

        syntax::Assign* parse_assign(int index, lexic::position pos);
        syntax::Read* parse_read(int index, lexic::position pos);
        syntax::Data* parse_data(int index, lexic::position pos);
        syntax::Print* parse_print(int index, lexic::position pos);
        syntax::Goto* parse_goto(int index, lexic::position pos);
        syntax::If* parse_if(int index, lexic::position pos);
        syntax::For* parse_for(int index, lexic::position pos);
        syntax::Next* parse_next(int index, lexic::position pos);
        syntax::Dim* parse_dim(int index, lexic::position pos);
        syntax::Def* parse_def(int index, lexic::position pos);
        syntax::Gosub* parse_gosub(int index, lexic::position pos);
        syntax::Return* parse_return(int index, lexic::position pos);
        syntax::Rem* parse_rem(int index, lexic::position pos);
        syntax::End* parse_end(int index, lexic::position pos);

        syntax::Exp* parse_exp(void);
        syntax::Eb* parse_eb(void);
        syntax::Operator* parse_operator(void);
        syntax::Num* parse_snum(void);
        syntax::Num* parse_num(void);
        syntax::Var* parse_var(void);
        syntax::Call* parse_call(void);
        syntax::Array* parse_array(void);
        syntax::Pitem* parse_pitem(void);

        enum class method {REQUIRED, OPTIONAL, LOOKAHEAD};
        bool consume(lexic::type type, method m);

        bool file_end();
};

} // namespace syntax

#endif // SYNTAX_ANALYSER_HPP

/*struct state {
    int id;
    bool accepting;
    bool operator==(const struct state& st) {
        return (id == st.id) && (accepting == st.accepting);
    }
};

struct transition {
    struct state current;
    bool input_is_terminal;
    int input;
    struct state next;
};*/

/*std::string type_name(lexic::type t);
bool FSM_program(lexic::LexicalAnalyser& lex);
bool FSM_exp(lexic::LexicalAnalyser& lex);
bool FSM_num(lexic::LexicalAnalyser& lex);
bool FSM_var(lexic::LexicalAnalyser& lex);
bool FSM_eb(lexic::LexicalAnalyser& lex);
int syntax_read(std::ifstream& file);*/



            // Para cada transição no reconhecedor utilizado
                // Filtra o estado da transição
                    // Verifica se a entrada corresponde a alguma transição de terminal
                        // Se sim, atribui a transição e consome próximo lexema
                    // Verifica se a entrada corresponde a alguma transição de não terminal
                        // Se sim, chama a submáquina de estados correspondente
                
            // Se houve transição realizada
                // Atualiza o estado
                // Segue o loop
            
            // Senão
                // Se o estado corrente é de aceitação
                    // Retorna true
                // Senão
                    // Retorna false
