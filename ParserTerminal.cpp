/// Copyright 2018 Andrew Hu
#include<cassert>
#include<memory>

#include"Parser.h"
#include"ExprAST.h"
#include"interp.h"
#include"Type.h"

using std::unique_ptr;
using std::make_unique;
using std::move;
using std::string;
using dep::Parser;
using dep::IntExprAST;
using dep::FloatExprAST;
using dep::ExprAST;
using dep::PrototypeAST;
using dep::FunctionAST;
using dep::IfExprAST;
using dep::BoolExprAST;

static unique_ptr<ExprAST> LogError(const char* Str);

unique_ptr<IntExprAST> Parser::ParseIntExpr() {
    assert(lex.peekCheck(tok_long) && "Parser was asked to parse integer"
            ", but the current token is not an int");
    auto node = make_unique<IntExprAST>(lex.IntVal());
    lex.expect(tok_long);
    return move(node);
}

unique_ptr<FloatExprAST> Parser::ParseFloatExpr() {
    assert(lex.peekCheck(tok_double) && "Parser was asked to parse float"
        ", but the current token is not a float");
    auto node = make_unique<FloatExprAST>(lex.FloatVal());
    lex.expect(tok_double);
    return move(node);
}

unique_ptr<BoolExprAST> Parser::ParseBoolExpr() {
    assert((lex.peekCheck(tok_true) || lex.peekCheck(tok_false)) && "Parser was asked parse "
        "boolean constant, but the current token is not a boolean");
    auto leaf = make_unique<BoolExprAST>(lex.IntVal());
    lex.expectBool(nullptr);
    return move(leaf);
}


unique_ptr<ExprAST> Parser::ParseIdentifierExpr() {
    assert(lex.peekCheck(tok_identifier) && "Parser asked to parse identifier"
        ", but current token is not an identifier");
    string identifierCopy = lex.IdentifierStr();
    // Now that we know what the identifier, let's "peek" at the next token, is it
    // an opening parenthesis? Although this consumes the next token, it's still
    // stored in the lexer, so we can just call the appropriate method later
    lex.expect(tok_identifier);
    // Check if it's a variable
    if (!lex.checkAdvance(tok_open_paren))
        return make_unique<VariableExprAST>(identifierCopy);

    // function call, so parse the arguments
    // int i = fib( <<<a>>>, b );
    //                 ^ Parser is here right now
    // FIXME Use a Tuple here to store the args, actually just implement the Tuple parser and use that 
    std::vector<unique_ptr<ExprAST>> args;
    while (!lex.checkAdvance(tok_close_paren)) {
        unique_ptr<ExprAST> currArg = ParseExpression();
        if (currArg == nullptr)
            return nullptr;
        args.push_back(move(currArg));

        // Logical short circuit will exit early on a correctly placed comma or
        // the closing paren, but will error if a comma was forgotten
        if (!lex.checkAdvance(tok_comma) && !lex.peekCheck(tok_close_paren)) {
            lex.LogError("Did you forget a comma?\nExpected ',' or ')'\n");
        }
    }
    return make_unique<CallExprAST>(identifierCopy, make_unique<TupleExprAST>(std::move(args)));
}

unique_ptr<PrototypeAST> Parser::ParsePrototype() {
    string name;
    if (!lex.expectIdent(&name))
        return nullptr;

    lex.expect(tok_open_paren);
    std::vector<string> types;
    std::vector<string> params;
    while (!lex.checkAdvance(tok_close_paren)) {
        string type;
        if (!lex.expectIdent(&type)) return nullptr;
        //TODO: conver string type to an actual type

        string name;
        if (!lex.expectIdent(&name)) return nullptr;
        params.push_back(name);

        // Logical short circuit will exit early on a correctly placed comma or
        // the closing paren, but will error if a comma was forgotten
        if (!lex.checkAdvance(tok_comma) && !lex.peekCheck(tok_close_paren)) {
            lex.LogError("Did you forget a comma?\nExpected ',' or ')'\n");
        }
    }
    return make_unique<PrototypeAST>(name, types, params);
}

unique_ptr<PrototypeAST> Parser::ParseExtern() {
    lex.expect(tok_extern);
    return ParsePrototype();
}

static unique_ptr<ExprAST> LogError(const char* Str) {
    std::cerr << Str << std::endl;
    return nullptr;
}