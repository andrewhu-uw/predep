#ifndef LEXER_H
#define LEXER_H

#include <cassert>

#include<string>
#include<map>
#include<memory>

#include"interp.h"

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
        Lexer() : intVal_(0), floatVal_(0.0), identifierStr_(""), unknownToken_(""), currTok_(tok_default) {
            binopPrecedence_[tok_plus] = 20;
            binopPrecedence_[tok_minus] = 20;
            binopPrecedence_[tok_times] = 40;
            binopPrecedence_[tok_divide] = 40;
            keywords["("] = tok_open_paren;
            keywords[")"] = tok_close_paren;
            keywords[","] = tok_comma;
            keywords["+"] = tok_plus;
            keywords["-"] = tok_minus;
            keywords["*"] = tok_times;
            keywords["/"] = tok_divide;
            keywords[";"] = tok_semicolon;
        }
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

        /*!	Return the type of next token from standard input.
            output member corresponding to the return value is set
            WARNING: return value may not be a valid token if the input could not be
            recognized*/
    private:
        Token getToken() {
            // FIXME we need a better solution than a static local variable
            static int LastChar = ' ';
            bool hasDecimal = false;

            // Skip any whitespace.
            while (isspace(LastChar))
                LastChar = getchar();

            if (isalpha(LastChar)) { // identifier: [a-zA-Z][a-zA-Z0-9]*
                identifierStr_ = LastChar;
                while (isalnum((LastChar = getchar())))
                    identifierStr_ += LastChar;

                if (identifierStr_ == "fn")
                    return Token::tok_fn;
                if (identifierStr_ == "extern")
                    return Token::tok_extern;
                return Token::tok_identifier;
            }

            if (LastChar == '.') hasDecimal = true;
            if (isdigit(LastChar) || LastChar == '.') { // Number: [0-9.]+
                std::string NumStr;
                do {
                    NumStr += LastChar;
                    LastChar = getchar();
                    if (LastChar == '.') hasDecimal = true;
                } while (isdigit(LastChar) || LastChar == '.');
                char* useless;
                if (hasDecimal) {
                    floatVal_ = strtod(NumStr.c_str(), nullptr);
                    return Token::tok_double;
                }
                else {
                    intVal_ = strtol(NumStr.c_str(), &useless, 10);
                    return Token::tok_long;
                }
            }

            if (LastChar == '#') {
                // Comment until end of line.
                do
                    LastChar = getchar();
                while (LastChar != EOF && LastChar != '\n' && LastChar != '\r');

                if (LastChar != EOF)
                    return getToken();
            }

            // Check for end of file.  Don't eat the EOF.
            if (LastChar == EOF)
                return Token::tok_eof;

            Token syntax_tok = tok_default;
            if (LastChar == '(')
                syntax_tok = Token::tok_open_paren;
            if (LastChar == ')')
                syntax_tok = Token::tok_close_paren;
            if (LastChar == ',')
                syntax_tok = Token::tok_comma;
            if (LastChar == '+')
                syntax_tok = Token::tok_plus;
            if (LastChar == '-')
                syntax_tok = Token::tok_minus;
            if (LastChar == '*')
                syntax_tok = Token::tok_times;
            if (LastChar == '/')
                syntax_tok = Token::tok_divide;
            if (LastChar == ';')
                syntax_tok = Token::tok_semicolon;

            if (syntax_tok != tok_default) {
                LastChar = getchar();
                return syntax_tok;
            }

            // Otherwise, just return the character as its ascii value.
            assert(false && "Should never get here");
            int ThisChar = LastChar;
            LastChar = getchar();
            return static_cast<Token>(ThisChar);
        }
    };
}

#endif // !LEXER_H