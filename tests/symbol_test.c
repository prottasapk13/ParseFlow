
#include <stdio.h>
#include "src/symbol_table/symbol_table.h"

int main(void)
{
    initSymbolTable();

    printf("=== Symbol Table Test ===\n");

    printf("\n1. Inserting variables:\n");
    printf("Insert x: %s\n",
           insertSymbol("x", "int", 1) ? "Success" : "Failed");

    printf("Insert y: %s\n",
           insertSymbol("y", "float", 2) ? "Success" : "Failed");

    printf("\n2. Testing redeclaration:\n");
    printf("Insert x again: %s\n",
           insertSymbol("x", "int", 3) ? "Success" : "Redeclaration detected");

    printf("\n3. Testing variable lookup:\n");
    printf("Lookup x: %s\n",
           lookupSymbol("x") ? "Found" : "Not found");

    printf("Lookup z: %s\n",
           lookupSymbol("z") ? "Found" : "Not found");

    printf("\n4. Testing nested scope:\n");
    enterScope();

    printf("Insert x in inner scope: %s\n",
           insertSymbol("x", "float", 5) ? "Success" : "Failed");

    printf("Lookup x in inner scope: %s\n",
           lookupSymbol("x")->type);

    printf("\n5. Symbol Table:\n");
    printSymbolTable();

    exitScope();

    printf("\n6. After exiting inner scope:\n");
    printf("Lookup x: %s\n", lookupSymbol("x")->type);

    freeSymbolTable();

    printf("\n=== Test Completed ===\n");

    return 0;
}
