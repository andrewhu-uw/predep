#ifndef LEXER_H
#define LEXER_H

#include <cassert>

#include<string>
#include<sstream>
#include<map>
#include<memory>

#include "ExprAST.h"
#include "Token.h"

namespace dep {
    class Lexer {
        long intVal_;
        double floatVal_;
        std::string identifierStr;
        std::string unknownToken;
        Token currTok;
        std::map<Token, int> binopPrecedence_;
        std::map<std::string, Token> keywords;

        //! Throw away currTok, load a new Token from the stream
        bool advanceToken() { currTok = getToken(); return true; }
    public:
        Lexer();
        long IntVal() { return intVal_; }
        double FloatVal() { return floatVal_; }
        std::string IdentifierStr() { return identifierStr; }
        std::string UnknownToken() { return unknownToken; }

        int getPrecedence() {
            if (!isascii(currTok) || binopPrecedence_.find(currTok) == binopPrecedence_.end())
                return -1;
            return binopPrecedence_[currTok];
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
                return t == currTok; 
        }
        //! If currTok == t, advance, else do nothing
        bool checkAdvance(Token t) { return peekCheck(t) ? advanceToken() : false; }
        //! If checkAdvance fails, write the error and return false
        bool expect(Token t) { if (checkAdvance(t)) return true; else LogErrorExpected(t, currTok); return false; }
        //! Is currTok and operator?
        bool checkOp() { return currTok > tok_op_begin && currTok < tok_op_end; }
        //! expect an operator, output value of operator through parameter iff return true
        bool expectOp(Token* op_ptr) { 
            if (checkOp()) {
                if (op_ptr) *op_ptr = currTok;
                advanceToken();
                return true;
            } else {
                LogErrorExpected(tok_op_begin, currTok);
                return false;
            }
        }

        bool checkBool() { return currTok == tok_false || currTok == tok_true; }
        bool expectBool(Token* tok_ptr) {
            if (checkBool()) {
                if (tok_ptr) *tok_ptr = currTok;
                advanceToken();
                return true;
            } else {
                std::stringstream err;
                err << "Expected a boolean value instead found token ";
                err << currTok;
                LogError(err.str().c_str());
            }
        }

        bool expectIdent(std::string* identifierPtr) { 
            if (peekCheck(tok_identifier)) { *identifierPtr = identifierStr; advanceToken(); return true; }
            else { LogErrorExpected(tok_identifier, currTok); return false; }
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