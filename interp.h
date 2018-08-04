#ifndef INTERP_H
#define INTERP_H

#include <map>

#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/IRBuilder.h"

#include "Type.h"
#include "Parser.h"

namespace dep {
    class Interp {
        Parser p;
        llvm::LLVMContext context;
        llvm::IRBuilder<> builder;
        std::map<std::string, llvm::Value*> namedValues;
        std::map<std::string, Type> types;
    public:
        Interp() : builder(context) {}
    };
}

#endif