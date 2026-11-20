
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_table.h"

static Scope *currentScope = NULL;

void initSymbolTable(void)
{
    currentScope = malloc(sizeof(Scope));

    if (currentScope == NULL) {
        printf("Memory allocation failed!\n");
        exit(1);
    }

    currentScope->level = 0;
    currentScope->symbols = NULL;
    currentScope->parent = NULL;
}

void enterScope(void)
{
    Scope *newScope = malloc(sizeof(Scope));

    if (newScope == NULL) {
        printf("Memory allocation failed!\n");
        exit(1);
    }

    newScope->level = currentScope->level + 1;
    newScope->symbols = NULL;
    newScope->parent = currentScope;

    currentScope = newScope;
}

void exitScope(void)
{
    if (currentScope == NULL || currentScope->parent == NULL)
        return;

    Scope *oldScope = currentScope;
    Symbol *symbol = oldScope->symbols;

    while (symbol != NULL) {
        Symbol *next = symbol->next;
        free(symbol->name);
        free(symbol->type);
        free(symbol);
        symbol = next;
    }

    currentScope = oldScope->parent;
    free(oldScope);
}

int insertSymbol(const char *name, const char *type, int line)
{
    if (currentScope == NULL)
        return 0;

    Symbol *existing = currentScope->symbols;

    while (existing != NULL) {
        if (strcmp(existing->name, name) == 0)
            return 0;

        existing = existing->next;
    }

    Symbol *newSymbol = malloc(sizeof(Symbol));

    if (newSymbol == NULL) {
        printf("Memory allocation failed!\n");
        exit(1);
    }

    newSymbol->name = strdup(name);
    newSymbol->type = strdup(type);
    newSymbol->scope_level = currentScope->level;
    newSymbol->line_number = line;

    newSymbol->next = currentScope->symbols;
    currentScope->symbols = newSymbol;

    return 1;
}

Symbol *lookupSymbol(const char *name)
{
    Scope *scope = currentScope;

    while (scope != NULL) {
        Symbol *symbol = scope->symbols;

        while (symbol != NULL) {
            if (strcmp(symbol->name, name) == 0)
                return symbol;

            symbol = symbol->next;
        }

        scope = scope->parent;
    }

    return NULL;
}

void printSymbolTable(void)
{
    printf("\nSymbol Table\n");
    printf("---------------------------------------------\n");
    printf("%-15s %-10s %-10s %-10s\n",
           "Name", "Type", "Scope", "Line");
    printf("---------------------------------------------\n");

    Scope *scope = currentScope;

    while (scope != NULL) {
        Symbol *symbol = scope->symbols;

        while (symbol != NULL) {
            printf("%-15s %-10s %-10d %-10d\n",
                   symbol->name,
                   symbol->type,
                   symbol->scope_level,
                   symbol->line_number);

            symbol = symbol->next;
        }

        scope = scope->parent;
    }

    printf("---------------------------------------------\n");
}

void freeSymbolTable(void)
{
    while (currentScope != NULL) {
        Scope *parent = currentScope->parent;
        Symbol *symbol = currentScope->symbols;

        while (symbol != NULL) {
            Symbol *next = symbol->next;
            free(symbol->name);
            free(symbol->type);
            free(symbol);
            symbol = next;
        }

        free(currentScope);
        currentScope = parent;
    }
}
