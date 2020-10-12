#ifndef LEXICAL_ANALYZER_HPP
#define LEXICAL_ANALYZER_HPP

#include <fstream>
#include <string>
#include <exception>

#include "ASCIIClassifier.hpp"

enum token_type {
    IDENTIFIER, // Identificador
    INTEGER,    // Sequência numérica
//Palavras reservadas:
    LET,        // Palavra reservada "LET"
    FN,         // Palavra reservada "FN"
    DEF,        // Palavra reservada "DEF"
    READ,       // Palavra reservada "READ"
    DATA,       // Palavra reservada "DATA"
    PRINT,      // Palavra reservada "PRINT"
    GO,         // Palavra reservada "GO"
    TO,         // Palavra reservada "TO"
    GOTO,       // Palavra reservada "GOTO"
    IF,         // Palavra reservada "IF"
    THEN,       // Palavra reservada "THEN"
    FOR,        // Palavra reservada "FOR"
    STEP,       // Palavra reservada "STEP"
    NEXT,       // Palavra reservada "NEXT"
    DIM,        // Palavra reservada "DIM"
    GOSUB,      // Palavra reservada "GOSUB"
    RETURN,     // Palavra reservada "RETURN"
    REM,        // Palavra reservada "REM"
    END,        // Palavra reservada "END"
// Símbolos:
    ADD,        // '+'  Operador de adição
    SUB,        // '-'  Operador de subtração
    MUL,        // '*'  Operador de multiplicação
    DIV,        // '/'  Operador de divisão
    POW,        // '^'  Operador de potenciação
    EQL,        // '='  Operador de igualdade
    NEQ,        // '<>' Operador de diferença
    LTN,        // '<'  Operador menor que
    GTN,        // '>'  Operador maior que
    LEQ,        // '<=' Operador menor que ou igual a
    GEQ,        // '>=' Operador maior que ou igual a
    COM,        // ','  Vírgula
    PNT,        // '.'  Ponto
    PRO,        // '('  Abre parênteses
    PRC,        // ')'  Fecha parênteses
    DQT,        // '"'  Aspas duplas
    EXD,        // 'E'  Expoente decimal para notação exponencial
// Funções prédefinidas
    SIN,        // Função Seno
    COS,        // Função Cosseno
    TAN,        // Função Tangente
    ATN,        // Função Arco-Tangente
    EXP,        // Função Exponencial
    ABS,        // Função Valor Absoluto
    LOG,        // Função Logarítmo
    SQR,        // Função Raíz Quadrada
    INT,        // Função Valor Inteiro
    RND         // Função Valor Aleatório
};

class lexical_exception: public std::exception {
    public:
        lexical_exception(position& loc, const char* error_message)
            : loc(loc), error_message(error_message) {
            exception();
        }

        std::string message() {
            return loc.position_str() + ": " + error_message;
        }

    private:
        position& loc;
        const char* error_message;
};

class token {
    public:
        token_type type;
        std::string value;
        position pos;

        token():
            value("") {}

        void add_char(char c) {
            std::string new_value = value + c;
            value = new_value;
        }

        void set_position(position& p) {
            pos.line = p.line;
            pos.column = p.column;
        }
};

class LexicalAnalyser {
    public:
        LexicalAnalyser(std::ifstream& file);
        token get_next();    

    private:
        token extract_token();
        token_type categorize_token(std::string& value);

        std::ifstream& file;
        ASCIIClassifier ac;
};

#endif // LEXICAL_ANALYZER_HPP