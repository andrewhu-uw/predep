#ifndef INTERP_H
#define INTERP_H

namespace dep {
	// The lexer returns tokens [0-255] if it is an unknown character, otherwise one
	// of these for known things.
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

        // syntactical
        tok_open_paren,
        tok_close_paren,
        tok_comma,
        tok_semicolon,

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