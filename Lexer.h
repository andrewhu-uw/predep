#ifndef LEXER_H
#define LEXER_H

#include <cassert>

#include<string>
#include<map>
#include<memory>

#include"interp.h"
#include"ExprAST.h"

namespace dep {
    class Lexer {
        long intVal_;
        double floatVal_;
        std::string identifierStr_;
        std::string unknownToken_;
        Token currTok_;
        std::map<Token, int> binopPrecedence_;
        std::map<std::string, Token> keywords;

        //! Throw away currTok, load a new Token from the stream
        bool advanceToken() { currTok_ = getToken(); return true; }
    public:
        Lexer();
        long intVal() { return intVal_; }
        double floatVal() { return floatVal_; }
        std::string identifierStr() { return identifierStr_; }
        std::string unknownToken() { return unknownToken_; }

        int getPrecedence() {
            if (!isascii(currTok_) || binopPrecedence_.find(currTok_) == binopPrecedence_.end())
                return -1;
            return binopPrecedence_[currTok_];
        }

        std::unique_ptr<ExprAST> LogError(const char* Str) {
            fprintf(stderr, Str);
            fprintf(stderr, "\n");
            return nullptr;
        }

        std::unique_ptr<ExprAST> LogErrorExpected(Token expected, Token actual) {
            fprintf(stderr, "Error: Expected token %d, instead found token %d\n", expected, actual);
            fprintf(stderr, "\n");
            return nullptr;
        }
        
        void doNothing() { std::cout << this; }

        //! Is the currTok == t?
        // FIXME: tok_unknown is not well integrated yet
        bool peekCheck(Token t) { 
            if (t == tok_unknown) 
                return t >= 0; 
            else 
                return t == currTok_; 
        }
        //! If currTok == t, advance, else do nothing
        bool checkAdvance(Token t) { return peekCheck(t) ? advanceToken() : false; }
        //! If checkAdvance fails, write the error and return false
        bool expect(Token t) { if (checkAdvance(t)) return true; else LogErrorExpected(t, currTok_); return false; }
        //! Is currTok and operator?
        bool checkOp() { return currTok_ > tok_op_begin && currTok_ < tok_op_end; }
        //! expect an operator, output value of operator through parameter iff return true
        bool expectOp(Token* op_ptr) { 
            if (checkOp()) {
                *op_ptr = currTok_;
                advanceToken();
                return true;
            } else {
                LogErrorExpected(tok_op_begin, currTok_);
                return false;
            }
        }

        bool expectIdent(std::string* identifierPtr) { 
            if (checkAdvance(tok_identifier)) { *identifierPtr = identifierStr_; return true; } 
            else { LogErrorExpected(tok_identifier, currTok_); return false; }
        }

        bool clearUntil(Token t) {
            while (!peekCheck(t))
                advanceToken();
            return true;
        }

        /*!	Return the type of next token from standard input.
            output member corresponding to the return value is set
            WARNING: return value may not be a valid token if the input could not be
            recognized*/
    private:
        Token getToken();
    };
}

#endif // !LEXER_H