/// Copyright 2018 Andrew Hu
#ifndef TOKEN_H
#define TOKEN_H

namespace dep {

    enum Token {
        tok_default = -200,
        tok_eof = -1,
        tok_unknown = -100,

        // commands
        tok_fn = -2,
        tok_extern,

        // primary
        tok_identifier,
        tok_double,
        tok_long,
        tok_true,
        tok_false,

        tok_if,
        tok_else,
        tok_for,
        tok_while,
        tok_let, // Probably won't be used
        tok_in,

        // syntactical
        tok_open_paren,
        tok_close_paren,
        tok_open_curly,
        tok_close_curly,
        tok_comma,
        tok_semicolon,
        tok_colon,
        tok_equals,

        //operators
        tok_op_begin,

        tok_plus,
        tok_minus,
        tok_times,
        tok_divide,
        tok_less_than,

        tok_op_end,
    };
}

#endif