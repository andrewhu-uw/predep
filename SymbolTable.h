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
        std::map<std::string, Type> idTable;
        std::string scopeName;
    public:
        SymbolTable() {}
        /*void enter(std::string id, Type type) {
            assert(idTable.find(id) == idTable.end() && "This identifier has already been entered into the idTable");
            idTable.insert(std::pair<std::string, Type>(id, type));
        }*/
    };
}

#endif