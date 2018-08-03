/// Copyright 2018 Andrew Hu
#ifndef FUNCTION_AST_H
#define FUNCTION_AST_H

#include<iostream>
#include<vector>
#include<memory>
#include<string>

#include "Type.h"
#include "ExprAST.h"
#include "StatementAST.h"

namespace dep {
    /*!  This class represents the "prototype" for a function,
    which captures its name, and its argument names (thus implicitly the number
    of arguments the function takes).*/
    class PrototypeAST {
        std::string Name;
        std::vector<std::string> Types;
        std::vector<std::string> Args;

    public:
        PrototypeAST(const std::string &name, std::vector<std::string> Types, std::vector<std::string> Args)
            : Name(name), Types(std::move(Types)), Args(std::move(Args)) {}

        const std::string &getName() const { return Name; }
    };

    //! This class represents a function definition itself.
    class FunctionAST {
        std::unique_ptr<PrototypeAST> Proto;
        std::unique_ptr<ExprAST> Body;

    public:
        FunctionAST(std::unique_ptr<PrototypeAST> Proto,
            std::unique_ptr<ExprAST> Body)
            : Proto(std::move(Proto)), Body(std::move(Body)) {}
    };
}

#endif