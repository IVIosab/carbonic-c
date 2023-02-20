%{
#include <iostream>
#include <fstream>
using namespace std;

// Include the lexer
#include "lexer.cpp"

int main() {
  // Open the input file
  ifstream input_file("program.txt");
  if (!input_file.is_open()) {
    cout << "Could not open input file" << endl;
    return 1;
  }

  // Tokenize the input
  int token;
  while ((token = yylex())) {
    // Ignore whitespace and comments
    if (token != TK_WHITESPACE && token != TK_COMMENT) {
      // Print the token type and value
      cout << "Token: " << yytext << endl;
    }
  }

  // Close the input file
  input_file.close();

  return 0;
}
%}

