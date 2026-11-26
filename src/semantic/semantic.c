
#include <stdio.h>
#include <string.h>
#include "semantic.h"
#include "../symbol_table/symbol_table.h"

static int semanticErrors = 0;

static const char *checkExpression(ASTNode *node)
{
    if (node == NULL)
        return "unknown";

    if (node->type == NODE_INTEGER)
        return "int";

    if (node->type == NODE_FLOAT)
        return "float";

    if (node->type == NODE_BOOLEAN)
        return "bool";

    if (node->type == NODE_IDENTIFIER) {
        Symbol *symbol = lookupSymbol(node->value);

        if (symbol == NULL) {
            printf("Semantic Error at line %d: Undeclared variable '%s'\n",
                   node->line_number, node->value);

            semanticErrors++;
            return "unknown";
        }

        return symbol->type;
    }

    if (node->type == NODE_UNARY) {
        const char *type = checkExpression(node->left);

        if (strcmp(node->value, "!") == 0) {
            if (strcmp(type, "bool") != 0 &&
                strcmp(type, "unknown") != 0) {

                printf("Semantic Error at line %d: Logical NOT requires bool\n",
                       node->line_number);

                semanticErrors++;
            }

            return "bool";
        }

        if (strcmp(type, "int") != 0 &&
            strcmp(type, "float") != 0 &&
            strcmp(type, "unknown") != 0) {

            printf("Semantic Error at line %d: Unary minus requires a number\n",
                   node->line_number);

            semanticErrors++;
        }

        return type;
    }

    if (node->type == NODE_BINARY) {
        const char *left = checkExpression(node->left);
        const char *right = checkExpression(node->right);
        const char *op = node->value;

        if (strcmp(op, "&&") == 0 || strcmp(op, "||") == 0) {
            if ((strcmp(left, "bool") != 0 &&
                 strcmp(left, "unknown") != 0) ||
                (strcmp(right, "bool") != 0 &&
                 strcmp(right, "unknown") != 0)) {

                printf("Semantic Error at line %d: Logical operators require bool operands\n",
                       node->line_number);

                semanticErrors++;
            }

            return "bool";
        }

        if (strcmp(op, "==") == 0 || strcmp(op, "!=") == 0 ||
            strcmp(op, "<") == 0 || strcmp(op, ">") == 0 ||
            strcmp(op, "<=") == 0 || strcmp(op, ">=") == 0) {

            return "bool";
        }

        if (strcmp(left, "bool") == 0 ||
            strcmp(right, "bool") == 0) {

            printf("Semantic Error at line %d: Arithmetic operators require numeric operands\n",
                   node->line_number);

            semanticErrors++;
            return "unknown";
        }

        if (strcmp(left, "float") == 0 ||
            strcmp(right, "float") == 0) {

            return "float";
        }

        return left;
    }

    return "unknown";
}

static void analyzeList(ASTNode *node);

static void analyzeNode(ASTNode *node)
{
    if (node == NULL)
        return;

    switch (node->type) {

        case NODE_DECLARATION: {
            ASTNode *identifier = node->left;

            if (!insertSymbol(identifier->value, node->value,
                              node->line_number)) {

                printf("Semantic Error at line %d: Redeclaration of '%s'\n",
                       node->line_number, identifier->value);

                semanticErrors++;
            }

            if (node->right != NULL) {
                const char *valueType = checkExpression(node->right);

                if (strcmp(node->value, valueType) != 0 &&
                    !(strcmp(node->value, "float") == 0 &&
                      strcmp(valueType, "int") == 0) &&
                    strcmp(valueType, "unknown") != 0) {

                    printf("Semantic Error at line %d: Type mismatch in declaration of '%s'\n",
                           node->line_number, identifier->value);

                    semanticErrors++;
                }
            }

            break;
        }

        case NODE_ASSIGNMENT: {
            ASTNode *identifier = node->left;
            Symbol *symbol = lookupSymbol(identifier->value);

            if (symbol == NULL) {
                printf("Semantic Error at line %d: Undeclared variable '%s'\n",
                       identifier->line_number, identifier->value);

                semanticErrors++;
            } else {
                const char *valueType = checkExpression(node->right);

                if (strcmp(symbol->type, valueType) != 0 &&
                    !(strcmp(symbol->type, "float") == 0 &&
                      strcmp(valueType, "int") == 0) &&
                    strcmp(valueType, "unknown") != 0) {

                    printf("Semantic Error at line %d: Type mismatch in assignment to '%s'\n",
                           node->line_number, identifier->value);

                    semanticErrors++;
                }
            }

            break;
        }

        case NODE_PRINT:
            checkExpression(node->left);
            break;

        case NODE_IF:
        case NODE_WHILE: {
            const char *conditionType = checkExpression(node->left);

            if (strcmp(conditionType, "bool") != 0 &&
                strcmp(conditionType, "unknown") != 0) {

                printf("Semantic Error at line %d: Condition must be bool\n",
                       node->line_number);

                semanticErrors++;
            }

            analyzeList(node->right);
            break;
        }

        case NODE_BLOCK:
            enterScope();
            analyzeList(node->left);
            exitScope();
            break;

        default:
            break;
    }
}

static void analyzeList(ASTNode *node)
{
    while (node != NULL) {
        analyzeNode(node);
        node = node->next;
    }
}

int analyzeSemantics(ASTNode *root)
{
    semanticErrors = 0;

    initSymbolTable();

    if (root != NULL && root->type == NODE_PROGRAM)
        analyzeList(root->left);
    else
        analyzeList(root);

    printf("\nSemantic Analysis:\n");

    if (semanticErrors == 0)
        printf("No semantic errors found!\n");
    else
        printf("Total semantic errors: %d\n", semanticErrors);

    printSymbolTable();
    freeSymbolTable();
    
     return semanticErrors;
}
