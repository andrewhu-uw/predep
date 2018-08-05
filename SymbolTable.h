/// Copyright 2018 Andrew Hu
#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <map>
#include <string>
#include <memory>

#include "Type.h"

namespace dep {
    class SymbolTable {
        std::map<std::string, std::shared_ptr<SymbolTable>> innerScopes;
        std::shared_ptr<SymbolTable> parentTable;
        std::map<std::string, Type> table;
        std::string scopeName;
    public:
        SymbolTable() {}
    };
}

#endif