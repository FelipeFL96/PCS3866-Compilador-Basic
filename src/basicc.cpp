#include <iostream>
/**/#include <string>

#include "ASCIIClassifier.hpp"
#include "LexicalAnalyser.hpp"
using namespace std;

int main(int argc, char* argv[]) {
    std::cout << "Bem-Vindo ao compilador basicc!" << std::endl;

    if (argc < 2) {
        cerr << "Uso esperado: basicc arquivo.txt" << endl;
        return 1;
    }

    char* filename = argv[1];
    std::ifstream file(filename, ios::binary);

    LexicalAnalyser lex(file);

    for (int i = 0; i < 10; i++) {
        token s = lex.get_next();
        cout << "LEXEME: " << s.value << endl;
    }
/*    cout << "Caractere que sobrou: ";
    lex.get_next();*/

    /*while (!file.eof()) {
        token t = lex.get_next();
        cout << "(" << c.pos.line << "," << c.pos.column << ") ";
        printf("[ %.2X  %c ]", (unsigned char) c.character, c.character);
        if (c.type == DIGIT) cout << "DIGIT" << endl;
        if (c.type == LETTER) cout << "LETTER" << endl;
        if (c.type == SPECIAL) cout << "SPECIAL" << endl;
        if (c.type == DELIMITER ) cout << "DELIMITER" << endl;
        if (c.type == UNKNOWN) cout << "UNKNOWN" << endl;
    }*/

    file.close();

    return 0;
}