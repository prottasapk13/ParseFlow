
#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

typedef struct Symbol {
    char *name;
    char *type;
    int scope_level;
    int line_number;
    struct Symbol *next;
} Symbol;

typedef struct Scope {
    int level;
    Symbol *symbols;
    struct Scope *parent;
} Scope;

void initSymbolTable(void);
void enterScope(void);
void exitScope(void);

int insertSymbol(const char *name, const char *type, int line);
Symbol *lookupSymbol(const char *name);

void printSymbolTable(void);
void freeSymbolTable(void);

#endif
