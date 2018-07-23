#ifndef LEXER_H
#define LEXER_H

#include<string>

#include"interp.h"

using std::string;

namespace dep {
    class Lexer {
        long intVal_;
        double floatVal_;
        string identifierStr_;
        Token currTok_;
    public:
        Lexer() : intVal_(0), floatVal_(0.0), identifierStr_("") {}
        long intVal() { return intVal_; }
        double floatVal() { return floatVal_; }
        string identifierStr() { return identifierStr_; }
        Token currTok() { return currTok_; }

        /*!	Return the type of next token from standard input.
        output parameter corresponding to the return value is set
        WARNING: return value may not be a valid token if the input could not be
        recognized
        pre: args are not null */
        Token getToken() {
            // not sure why this is static, but too scared to remove
            static int LastChar = ' ';
            bool hasDecimal = false;

            // Skip any whitespace.
            while (isspace(LastChar))
                LastChar = getchar();

            if (isalpha(LastChar)) { // identifier: [a-zA-Z][a-zA-Z0-9]*
                identifierStr_ = LastChar;
                while (isalnum((LastChar = getchar())))
                    identifierStr_ += LastChar;

                if (identifierStr_ == "def")
                    return Token::tok_def;
                if (identifierStr_ == "extern")
                    return Token::tok_extern;
                return Token::tok_identifier;
            }

            if (LastChar == '.') hasDecimal = true;
            if (isdigit(LastChar) || LastChar == '.') { // Number: [0-9.]+
                string NumStr;
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

            // Otherwise, just return the character as its ascii value.
            int ThisChar = LastChar;
            LastChar = getchar();
            return static_cast<Token>(ThisChar);
        }
    };
}

#endif // !LEXER_H