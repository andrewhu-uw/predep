/// Copyright 2018 Andrew Hu
//! All parses both parse and consume tokens
#ifndef PARSE_H
#define PARSE_H

#include<memory>

#include"ExprAST.h"
#include"Lexer.h"

using dep::Lexer;

namespace dep {
    class Parser {
        Lexer lex;
    public:
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

        std::unique_ptr<dep::ExprAST> ParsePrimary();

        std::unique_ptr<dep::ExprAST> ParseExpression();
    };
}
#endif