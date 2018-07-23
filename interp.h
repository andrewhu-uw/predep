#ifndef INTERP_H
#define INTERP_H

namespace dep {
	// The lexer returns tokens [0-255] if it is an unknown character, otherwise one
	// of these for known things.
	enum Token {
		tok_eof = -1,

		// commands
		tok_def = -2,
		tok_extern = -3,

		// primary
		tok_identifier = -4,
		tok_double = -5,
		tok_long = -6,
	};
}

#endif