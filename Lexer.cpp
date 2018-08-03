// Copyright 2018 Andrew Hu
#include "Lexer.h"

using dep::Token;
using dep::Lexer;

Lexer::Lexer() : intVal_(0), floatVal_(0.0), identifierStr(""), unknownToken(""), currTok(tok_default) {
    binopPrecedence_[tok_plus] = 20;
    binopPrecedence_[tok_minus] = 20;
    binopPrecedence_[tok_times] = 40;
    binopPrecedence_[tok_divide] = 40;
    binopPrecedence_[tok_less_than] = 10;
    keywords["("] = tok_open_paren;
    keywords[")"] = tok_close_paren;
    keywords[","] = tok_comma;
    keywords["+"] = tok_plus;
    keywords["-"] = tok_minus;
    keywords["*"] = tok_times;
    keywords["/"] = tok_divide;
    keywords[";"] = tok_semicolon;
    keywords["="] = tok_equals;
    keywords[":"] = tok_colon;
}

Token Lexer::getToken() {
    // FIXME we need a better solution than a static local variable
    static int LastChar = ' ';
    bool hasDecimal = false;

    // Skip any whitespace.
    while (isspace(LastChar))
        LastChar = getchar();

    if (isalpha(LastChar)) { // identifier: [a-zA-Z][a-zA-Z0-9]*
        identifierStr = LastChar;
        while (isalnum((LastChar = getchar())))
            identifierStr += LastChar;

        if (identifierStr == "fn")
            return Token::tok_fn;
        if (identifierStr == "extern")
            return Token::tok_extern;
        if (identifierStr == "if")
            return Token::tok_if;
        if (identifierStr == "else")
            return Token::tok_else;
        if (identifierStr == "true")
            return Token::tok_true;
        if (identifierStr == "false")
            return Token::tok_false;
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

    // TODO: Fix this ugly mess that assumes keywords are one character
    Token syntax_tok = tok_default;
    std::string keyword = std::string(1, static_cast<char>(LastChar));
    auto result = keywords.find(keyword);
    if (result != keywords.end())
        syntax_tok = result->second;

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