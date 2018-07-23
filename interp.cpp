/// Copyright 2018 Andrew Hu
#include<algorithm>
#include<cstdlib>
#include<cctype>
#include<memory>
#include<string>
#include<iostream>

#include"interp.h"
#include"ExprAST.h"
#include"Lexer.h"
#include"Parse.h"

using dep::Token;
using std::shared_ptr;
using std::string;
using dep::Lexer;



int main() {
    string buf;
	double dval;
	long ival;
	
    Lexer lex;
	Token input = lex.getToken();
	if (input == Token::tok_identifier) {
        std::cout << lex.identifierStr();
	} else if (input == Token::tok_double) {
        std::cout << lex.floatVal();
	} else {
		std::cout << lex.intVal();
	}

    dep::Parser p;
    //p.ParseParenExpr();
	std::cin.get();
}