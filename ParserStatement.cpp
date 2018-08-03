/// Copyright 2018 Andrew Hu

#include <memory>
#include <vector>

#include "Parser.h"
#include "StatementAST.h"
#include "FunctionAST.h"

using std::unique_ptr;
using std::make_unique;
using std::move;
using std::string;
using std::vector;

namespace dep {
    unique_ptr<StatementAST> Parser::ParseStatement() {
        if (lex.checkAdvance(tok_if)) {
            // TODO Parse if statement
        } else if (lex.checkAdvance(tok_while)) {
            // TODO parse while loop
        } else if (lex.checkAdvance(tok_for)) {
            // TODO parse for loop
        } else if (lex.peekCheck(tok_identifier)) {
            // We know there's an identifier, but don't know if it's a 
            // type, variable name, function call

            // Save that ident and move past it
            string first_ident;
            lex.expectIdent(&first_ident);

            if (lex.peekCheck(tok_open_paren)) {
                // Function call
                unique_ptr<TupleExprAST> argument_list = ParseTuple();
                unique_ptr<CallExprAST> call_expr = make_unique<CallExprAST>(first_ident, move(argument_list));
                return make_unique<CallStatementAST>(std::move(call_expr));
            }

            string& type_name = first_ident;
            string var_name;
            if (!lex.expectIdent(&var_name)) return nullptr;

            vector<string> props;
            if (lex.checkAdvance(tok_colon)) {
                // Specifying properties
                // Type name : Prop [ + Other] 
                string first_prop;
                lex.expectIdent(&first_prop);
                while (lex.checkAdvance(tok_plus)) {
                    string prop_name;
                    if (!lex.expectIdent(&prop_name)) return nullptr;
                    props.push_back(prop_name);
                }
            }

            auto def = make_unique<DefineAST>(type_name, var_name);

            if (lex.checkAdvance(tok_equals)) {
                // Init
                auto rval = ParseExpression();
                assert(lex.peekCheck(tok_semicolon) && "This should be the end of the statement");
                return make_unique<InitAST>(move(def), move(rval));
            } 
            // Def
            assert(lex.peekCheck(tok_semicolon) && "This should be the end of the statement");
            return move(def);
        }

        return make_unique<DefineAST>("Hello", "World");
    }
}