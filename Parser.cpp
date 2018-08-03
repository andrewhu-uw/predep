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

unique_ptr<ExprAST> Parser::ParseParenExpr() {
    // int i = ( 5 + 9 ) * 7
    //         ^ currTok is here
    // move from '(' to the inside
    assert(lex.peekCheck(tok_open_paren) && "Parser was asked to parse parentheses"
        ", but current token is not opening parentheses");
    lex.expect(tok_open_paren);
    auto insideExpr = ParseExpression();
    if (insideExpr == nullptr)
        return nullptr;

    // verify that ')' was there, and move past it.
    lex.expect(tok_close_paren);
    return insideExpr;
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
    return make_unique<CallExprAST>(identifierCopy, std::move(args));
}

unique_ptr<ExprAST> Parser::ParsePrimary() {
    if (lex.peekCheck(tok_identifier))
        return ParseIdentifierExpr();
    else if (lex.peekCheck(tok_double))
        return ParseFloatExpr();
    else if (lex.peekCheck(tok_long))
        return ParseIntExpr();
    else if (lex.peekCheck(tok_open_paren))
        return ParseParenExpr();
    else if (lex.peekCheck(tok_if))
        return ParseIfExpr();
    else
        return LogError("unknown token when expecting an expression");
}

unique_ptr<ExprAST> Parser::ParseExpression() {
    unique_ptr<ExprAST> LHS = ParsePrimary();
    if (!LHS) return nullptr;
    
    // precedence zero means that any operator can be applied to the right
    return ParseBinOpRHS(0, std::move(LHS));
}

unique_ptr<ExprAST> Parser::ParseBinOpRHS(int precedenceLHS, unique_ptr<ExprAST> LHS) {
    // FIXME recursion with a loop, gross I know
    while (true) {
        // if this is not an operator, the expression is over
        if (!lex.checkOp())
            return LHS;
        int curr_prec = lex.getPrecedence();

        // if it's precedence is too low, quit early
        // this should happen around here
        // a + b * c <<<+>>> d
        if (curr_prec < precedenceLHS)
            return LHS;

        // get the operator and move past it
        Token op;
        if (!lex.expectOp(&op)) {
            LogError("Thought we found a valid operator, but it was not. This should never happen.\n");
        }
        
        unique_ptr<ExprAST> RHS = ParsePrimary();
        if (!RHS) return nullptr;

        // This is the precedence of the operator just past RHS
        // a + b <<<op>>> ???
        int next_op_prec = lex.getPrecedence();

        if (curr_prec < next_op_prec) {
            // When our RH operator is more precedent, evaluate the RH first, but 
            // make the minimum precedence to continue just higher than the current operator,
            // that way we get (a)+(b*c)+(d) instead of (a)+(b*c+d)
            RHS = ParseBinOpRHS(precedenceLHS + 1, move(RHS));
            if (!RHS) return nullptr;
        }

        LHS = make_unique<BinaryExprAST>(op, move(LHS), move(RHS));
    }
}

unique_ptr<PrototypeAST> Parser::ParsePrototype() {
    string name;
    if (!lex.expectIdent(&name))
        return nullptr;

    lex.expect(tok_open_paren);
    std::vector<dep::Type> types;
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

unique_ptr<ExprAST> Parser::ParseIfExpr() {
    lex.expect(tok_if);
    lex.expect(tok_open_paren);

    unique_ptr<ExprAST> cond = ParseExpression();
    if (!cond) return nullptr;

    lex.expect(tok_close_paren);

    // Accept with curlies or not
    bool has_curlies = lex.checkAdvance(tok_open_curly);

    auto then_block = ParseExpression();
    if (!then_block) return nullptr;

    if (has_curlies) lex.expect(tok_close_curly);

    //TODO right now we only accept if else statements since functions are only one expression
    lex.expect(tok_else);
    if (has_curlies) lex.expect(tok_open_curly);

    auto else_block = ParseExpression();
    if (!else_block) return nullptr;

    if (has_curlies) lex.expect(tok_close_curly);

    return make_unique<IfExprAST>(move(cond), move(then_block), move(else_block));
}

unique_ptr<FunctionAST> Parser::ParseDefinition() {
    lex.expect(tok_fn);

    auto proto = ParsePrototype();
    if (!proto) return nullptr;

    auto body = ParseExpression();
    return body ? make_unique<FunctionAST>(move(proto), move(body)) : nullptr;
}

unique_ptr<PrototypeAST> Parser::ParseExtern() {
    lex.expect(tok_extern);
    return ParsePrototype();
}

unique_ptr<FunctionAST> Parser::ParseTopLevelExpr() {
    auto expr = ParseExpression();
    if (!expr) return nullptr;
    
    auto proto = make_unique<PrototypeAST>("__anon_expr", std::vector<dep::Type>(), std::vector<string>());
    return make_unique<FunctionAST>(move(proto), move(expr));
}

static unique_ptr<ExprAST> LogError(const char* Str) {
    std::cerr << Str << std::endl;
    return nullptr;
}