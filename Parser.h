// Copyright 2018 Andrew Hu
//! All parses both parse and consume tokens
#ifndef PARSE_H
#define PARSE_H

#include<memory>
#include<iostream>

#include "ExprAST.h"
#include "Lexer.h"
#include "FunctionAST.h"

using dep::Lexer;

namespace dep {
    class Parser {
        //int i;
        Lexer lex;
    public:
        Parser() { lex.expect(tok_default); }
        bool onToken(Token t) { return lex.peekCheck(t); }
        bool clearLine() { lex.clearUntil(tok_semicolon); return lex.checkAdvance(tok_semicolon); }

        //! Parses the current integer immediate
        //! @pre{ lex.currTok_ == tok_long }
        //! int i = <<<5>>>;
        //!            ^ Call when curr token is in the "wakas"
        std::unique_ptr<dep::IntExprAST> ParseIntExpr();

        //! Parses the current float immediate
        //! @pre { lex.currTok_ == tok_float }
        //! int i = <<<5.0>>>;
        //!            ^ Call when curr token is in the "wakas"
        std::unique_ptr<dep::FloatExprAST> ParseFloatExpr();

        //! Parses the current boolean constant
        //! @pre { lex.checkBool in { tok_true, tok_false } }
        //! bool b = <<<false>>>;
        std::unique_ptr<dep::BoolExprAST> ParseBoolExpr();

        //! Parses parentheses that have <i>not</i> been consumed
        //! yet
        //! @pre { lex.currTok_ == '(' }
        //! int i = <<<(>>> 5 + 9 ) * 7;
        //!            ^ Call when curr token is in the "wakas"
        std::unique_ptr<dep::ExprAST> ParseParenExpr();

        //! Parses something that starts with letters
        //! @pre { lex.currTok_ == tok_identifier
        //! int i = <<<count>>>;
        //! int i = <<<fib>>>( 0 );
        //!            ^ Call when curr token is in one of these
        std::unique_ptr<dep::ExprAST> ParseIdentifierExpr();

        std::unique_ptr<dep::TupleExprAST> ParseTuple();

        /*! Parse an entire rvalue
            int i = <<<5+3>>;
            int i = <<<fib(5) + 10>>>;
        */
        std::unique_ptr<dep::ExprAST> ParseExpression();

        /*! Parse one discrete expression, although not necessarily
            atomic
            int i = <<<0>>>;
            int i = <<<(5 * 3)>>>;
        */
        std::unique_ptr<dep::ExprAST> ParsePrimary();

        /*! Parse the entire right side of a binary operator recursively
            int i = 5 <<<* 3>>>;
            int i = 5 <<<* 3 + 9>>>;
            
            precedenceLHS represents the minimum operator precedence to continue parsing to the right
        */
        std::unique_ptr<dep::ExprAST> ParseBinOpRHS(int precedenceLHS, std::unique_ptr<dep::ExprAST> LHS);

        std::unique_ptr<dep::ExprAST> ParseIfExpr();

        std::unique_ptr<dep::PrototypeAST> ParsePrototype();

        std::unique_ptr<dep::FunctionAST> ParseDefinition();

        std::unique_ptr<dep::PrototypeAST> ParseExtern();

        std::unique_ptr<dep::FunctionAST> ParseTopLevelExpr();

        std::unique_ptr<dep::StatementAST> ParseStatement();

        std::unique_ptr<dep::BlockAST> ParseBlock();
    };
}
#endif