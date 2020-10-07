#include <iostream>
#include <fstream>
#include <exception>
#include <string>

#include "ASCIIClassifier.hpp"

#include "LexicalAnalyser.hpp"

#define debug(x) cout<<x<<endl;

using namespace std;

LexicalAnalyser::LexicalAnalyser(ifstream& file):
    file(file), ac(file) {
        cout << "O analisador léxico já está começando a existir!" << endl;
    }

token LexicalAnalyser::get_next() {
   token t;
   t.value = extract_token();
   return t;
}

string LexicalAnalyser::extract_token() {
    enum state {INITIAL, WORD, INT, SYMBOL};
    state current_state = INITIAL;
    
    string lexeme = "";
    bool token_completed = false;
    while (!token_completed) {
        ascii_character c = ac.get_next();
        switch (current_state) {
            case INITIAL:
                if (c.type == LETTER) {
                    lexeme += c.character;
                    current_state = WORD;
                }
                else if (c.type == DIGIT) {
                    lexeme += c.character;
                    current_state = INT;
                }
                else if (c.type == SPECIAL) {
                    lexeme += c.character;
                    if (c.character == '>' || c.character == '<')
                        current_state = SYMBOL;
                    else 
                        token_completed = true;
                }
                else if (c.type == DELIMITER) {
                    continue;
                }
                else {
                    debug("EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEPA!!!!!");
                }
                break;
            case WORD:
                if (c.type == LETTER || c.type == DIGIT) {
                    lexeme += c.character;
                    current_state = WORD;
                }
                else {
                    token_completed = true;
                }
                break;
            case INT:
                if (c.type == DIGIT) {
                    lexeme += c.character;
                    current_state = INT;
                }
                else {
                    token_completed = true;
                }
                break;
            case SYMBOL:
                if (c.type == SPECIAL && (c.character == '=' || c.character == '>')) {
                    lexeme += c.character;
                }
                else {
                    token_completed = true;
                }
        }
    }
    if (current_state != INITIAL)
        ac.back();
    return lexeme;
}