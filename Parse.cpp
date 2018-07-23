#include "Parse.h"
/// Copyright 2018 Andrew Hu
#include<memory>
#include<cassert>

#include"Parse.h"
#include"ExprAST.h"
#include"interp.h"

using std::unique_ptr;
using std::make_unique;
using std::move;
using dep::Parser;
using dep::IntExprAST;
using dep::FloatExprAST;
using dep::ExprAST;

static unique_ptr<ExprAST> LogError(const char* Str);

unique_ptr<IntExprAST> Parser::ParseIntExpr() {
    assert(lex.currTok() == tok_long && "Parser was asked to parse integer"
            ", but the current token is not an int");
    dep::Token result = lex.getToken();
    return make_unique<IntExprAST>(lex.intVal());
}

unique_ptr<FloatExprAST> Parser::ParseFloatExpr() {
    assert(lex.currTok() == tok_double && "Parser was asked to parse float"
        ", but the current token is not a float");
    dep::Token result = lex.getToken();
    return make_unique<FloatExprAST>(lex.floatVal());
}

unique_ptr<ExprAST> Parser::ParseParenExpr() {
    // int i = ( 5 + 9 ) * 7
    //         ^ currTok is here
    // move from '(' to the inside
    assert(lex.currTok() == '(' && "Parser was asked to parse parentheses"
        ", but current token is not opening parentheses");
    lex.getToken();
    auto insideExpr = ParseExpression();
    if (insideExpr == nullptr)
        return nullptr;

    if (lex.currTok() != ')')
        return LogError("Expected end of parentheses, didn't find it");
    // move from ')' to the next token
    lex.getToken();
    return insideExpr;
}

std::unique_ptr<dep::ExprAST> dep::Parser::ParseExpression() {
    return std::unique_ptr<dep::ExprAST>();
}

static unique_ptr<ExprAST> LogError(const char* Str) {
    fprintf(stderr, Str);
    return nullptr;
}