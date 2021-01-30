#include <iostream>
#include <string>
#include <string.h>

#include "lexic.hpp"

using namespace std;

string type2name(lexic::type t);
string ascii2name(lexic::ascii_type t);

void ascii_test(ifstream& file);
void lex_test(ifstream& file);
void stx_test(ifstream& file);
