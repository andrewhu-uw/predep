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
using dep::Parser;

class Dummy {
    int i;
    int j;
    int k;
public:
    void doNothing() { std::cout << this; }
};

void MainLoop() {
    std::cerr << "dep>";
    // Hang to wait for initial input

    Parser p;
    while (true) {
        if (p.onToken(Token::tok_fn)) {
            if (p.ParseDefinition()) std::cerr << "Parsed a definition" << std::endl;
        } else if (p.onToken(Token::tok_extern)) {
            if (p.ParseExtern()) std::cerr << "Parsed an extern prototype" << std::endl;
        } else {
            if (p.ParseTopLevelExpr()) std::cerr << "Parsed a top-level expression" << std::endl;;
        }
        std::cerr << "dep>";
        if (p.onToken(Token::tok_semicolon)) {
            p.clearSemicolon();
        }
    }
}

int main() {
    MainLoop();
}