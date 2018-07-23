#include "Parse.h"
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

std::unique_ptr<dep::ExprAST> dep::Parser::ParseIdentifierExpr() {
    assert(lex.currTok() == tok_identifier && "Parser asked to parse identifier"
        ", but current token is not an identifier");
    string identifierCopy = lex.identifierStr();
    // Now that we know what the identifier, let's "peek" at the next token, is it
    // an opening parenthesis? Although this consumes the next token, it's still
    // stored in the lexer, so we can just call the appropriate method later
    lex.getToken();
    // Check if it's a variable
    if (lex.currTok() != '(')
        return make_unique<VariableExprAST>(identifierCopy);

    // Looks like it's a function call, so parse the arguments
    // int i = fib<<<(>>> 0 );
    //               ^ Parser is here right now
    lex.getToken();
    std::vector<unique_ptr<ExprAST>> args;
    while (lex.currTok() != ')') {
        unique_ptr<ExprAST> currArg = ParseExpression();
        if (currArg == nullptr)
            return nullptr;
        args.push_back(move(currArg));

        if (lex.currTok() == ',') {
            lex.getToken();
        } else if (lex.currTok() != ')') {
            LogError("Expected closing parenthesis on function call");
        }
    }
    // Move past the closing parenthesis
    lex.getToken();
    return make_unique<CallExprAST>(identifierCopy, std::move(args));
}

std::unique_ptr<ExprAST> Parser::ParsePrimary() {
    switch (lex.currTok()) {
    default:
        return LogError("unknown token when expecting an expression");
    case tok_identifier:
        return ParseIdentifierExpr();
    case tok_double:
        return ParseFloatExpr();
    case tok_long:
        return ParseIntExpr();
    case '(':
        return ParseParenExpr();
    }
}

std::unique_ptr<dep::ExprAST> dep::Parser::ParseExpression() {
    return nullptr;
}

static unique_ptr<ExprAST> LogError(const char* Str) {
    fprintf(stderr, Str);
    return nullptr;
}