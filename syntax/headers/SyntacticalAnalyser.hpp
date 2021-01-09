#ifndef SYNTACTICAL_ANALYSER_HPP
#define SYNTACTICAL_ANALYSER_HPP

#include <iostream>
#include <string>

#include "syntax.hpp"
#include "lexic.hpp"
#include "LexicalAnalyser.hpp"

namespace syntax {

struct state {
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
};

/*enum class type {
    NUM,
    VAR,
    EB,
    EXP
};*/

class SyntacticalAnalyser {
    public:
        SyntacticalAnalyser(std::ifstream& file);

        syntax::Syntaxeme* get_next(void);

    private:
        std::ifstream& file;
        lexic::LexicalAnalyser lex;

        syntax::Assign* get_assign(int index);
        syntax::Goto* get_goto(int index);
};

} // namespace syntax

/*std::string type_name(lexic::type t);
bool FSM_program(lexic::LexicalAnalyser& lex);
bool FSM_exp(lexic::LexicalAnalyser& lex);
bool FSM_num(lexic::LexicalAnalyser& lex);
bool FSM_var(lexic::LexicalAnalyser& lex);
bool FSM_eb(lexic::LexicalAnalyser& lex);
int syntax_read(std::ifstream& file);*/

#endif // SYNTACTICAL_ANALYSER_HPP

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
