#include<algorithm>
#include<cstdlib>
#include<cctype>
#include<memory>
#include<string>
#include<iostream>

#include"interp.h"

using dep::Token;
using std::shared_ptr;
using std::string;

static Token gettok(string* IdentifierStr, double* NumVal, long* IntVal);

int main() {
	string buf;
	double dval;
	long ival;
	
	Token input = gettok(&buf, &dval, &ival);
	if (input == Token::tok_identifier) {
		std::cout << buf;
	} else if (input == Token::tok_double) {
		std::cout << dval;
	} else {
		std::cout << ival;
	}

	std::cin.get();
}

/// gettok - Return the type of next token from standard input.
/// output parameter corresponding to the return value is set
/// WARNING: return value may not be a valid token if the input could not be
/// recognized
/// pre: args are not null
static Token gettok(string* IdentifierStr, double* NumVal, long* IntVal) {
	// not sure why this is static, but too scared to remove
	static int LastChar = ' ';
	bool hasDecimal = false;

	// Skip any whitespace.
	while (isspace(LastChar))
		LastChar = getchar();

	if (isalpha(LastChar)) { // identifier: [a-zA-Z][a-zA-Z0-9]*
		*IdentifierStr = LastChar;
		while (isalnum((LastChar = getchar())))
			*IdentifierStr += LastChar;

		if (*IdentifierStr == "def")
			return Token::tok_def;
		if (*IdentifierStr == "extern")
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
			*NumVal = strtod(NumStr.c_str(), nullptr);
			return Token::tok_double;
		}
		else {
			*IntVal = strtol(NumStr.c_str(), &useless, 10);
			return Token::tok_long;
		}
	}

	if (LastChar == '#') {
		// Comment until end of line.
		do
			LastChar = getchar();
		while (LastChar != EOF && LastChar != '\n' && LastChar != '\r');

		if (LastChar != EOF)
			return gettok(IdentifierStr, NumVal, IntVal);
	}

	// Check for end of file.  Don't eat the EOF.
	if (LastChar == EOF)
		return Token::tok_eof;

	// Otherwise, just return the character as its ascii value.
	int ThisChar = LastChar;
	LastChar = getchar();
	return static_cast<Token>(ThisChar);
}