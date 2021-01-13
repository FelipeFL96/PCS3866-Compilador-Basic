#include <vector>

#include "syntax.hpp"

#include "SemanticAnalyser.hpp"

using namespace std;

SemanticAnalyser::SemanticAnalyser(ifstream& file):
    stx(file)
{}

void SemanticAnalyser::parse_expression() {
    syntax::Exp* exp = stx.get_exp();

    vector<syntax::Eb*> operands = exp->get_operands();
    vector<syntax::Operator*> operators = exp->get_operators();
    cout << "SEMÂNTICA TÁ NASCENDO!!" << endl;
}