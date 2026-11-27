
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tac.h"

static int tempCount = 0;
static int labelCount = 0;

static int scopeCount = 0;
static int currentScope = 0;

static int scopeStack[100];
static int scopeDepth = 0;

typedef struct Variable {
    char *name;
    char *tacName;
    int scope;
    struct Variable *next;
} Variable;

static Variable *variables = NULL;

static char *newName(const char *prefix, int number)
{
    char buffer[30];

    sprintf(buffer, "%s%d", prefix, number);

    return strdup(buffer);
}

static char *newTemp(void)
{
    return newName("t", ++tempCount);
}

static char *newLabel(void)
{
    return newName("L", ++labelCount);
}

static void enterTACScope(void)
{
    if (scopeDepth < 100) {
        currentScope = ++scopeCount;
        scopeStack[scopeDepth++] = currentScope;
    }
}

static void exitTACScope(void)
{
    if (scopeDepth == 0)
        return;

    Variable **ptr = &variables;

    while (*ptr != NULL) {
        if ((*ptr)->scope == currentScope) {
            Variable *temp = *ptr;
            *ptr = temp->next;

            free(temp->name);
            free(temp->tacName);
            free(temp);
        } else {
            ptr = &(*ptr)->next;
        }
    }

    scopeDepth--;
    currentScope = scopeDepth > 0
        ? scopeStack[scopeDepth - 1]
        : 0;
}

static char *resolveName(const char *name)
{
    Variable *var = variables;

    while (var != NULL) {
        if (strcmp(var->name, name) == 0)
            return strdup(var->tacName);

        var = var->next;
    }

    return strdup(name);
}

static void declareVariable(const char *name)
{
    Variable *var = malloc(sizeof(Variable));

    if (var == NULL)
        return;

    var->name = strdup(name);

    if (currentScope == 0) {
        var->tacName = strdup(name);
    } else {
        char buffer[256];

        snprintf(buffer, sizeof(buffer), "%s_scope%d",
                 name, currentScope);

        var->tacName = strdup(buffer);
    }

    var->scope = currentScope;
    var->next = variables;
    variables = var;
}

static char *generateExpression(ASTNode *node)
{
    if (node == NULL)
        return strdup("");

            if (node->type == NODE_IDENTIFIER)
        return resolveName(node->value);

    if (node->type == NODE_INTEGER ||
        node->type == NODE_FLOAT ||
        node->type == NODE_BOOLEAN)
        return strdup(node->value);

    if (node->type == NODE_UNARY) {
        char *operand = generateExpression(node->left);
        char *temp = newTemp();

        printf("%s = %s%s\n", temp, node->value, operand);

        free(operand);
        return temp;
    }

    if (node->type == NODE_BINARY) {
        char *left = generateExpression(node->left);
        char *right = generateExpression(node->right);
        char *temp = newTemp();

        printf("%s = %s %s %s\n",
               temp, left, node->value, right);

        free(left);
        free(right);

        return temp;
    }

    return strdup("");
}

static void generateStatement(ASTNode *node);

static void generateList(ASTNode *node)
{
    while (node != NULL) {
        generateStatement(node);
        node = node->next;
    }
}

static void generateStatement(ASTNode *node)
{
    if (node == NULL)
        return;

    switch (node->type) {

        case NODE_DECLARATION: {
    declareVariable(node->left->value);

    if (node->right != NULL) {
        char *value = generateExpression(node->right);
        char *name = resolveName(node->left->value);

        printf("%s = %s\n", name, value);

        free(name);
        free(value);
    }
    break;
}

      case NODE_ASSIGNMENT: {
    char *value = generateExpression(node->right);
    char *name = resolveName(node->left->value);

    printf("%s = %s\n", name, value);

    free(name);
    free(value);
    break;
}

        case NODE_PRINT: {
            char *value = generateExpression(node->left);

            printf("print %s\n", value);
            free(value);
            break;
        }

        case NODE_IF: {
            char *condition = generateExpression(node->left);
            char *elseLabel = newLabel();
            char *endLabel = NULL;

            printf("ifFalse %s goto %s\n", condition, elseLabel);
            free(condition);

            generateStatement(node->right);

            if (node->value != NULL &&
                strcmp(node->value, "if-else") == 0) {
                endLabel = newLabel();

                printf("goto %s\n", endLabel);
                printf("%s:\n", elseLabel);

                if (node->right != NULL)
                    generateStatement(node->right->next);

                printf("%s:\n", endLabel);
                free(endLabel);
            } else {
                printf("%s:\n", elseLabel);
            }

            free(elseLabel);
            break;
        }

        case NODE_WHILE: {
            char *startLabel = newLabel();
            char *endLabel = newLabel();

            printf("%s:\n", startLabel);

            char *condition = generateExpression(node->left);

            printf("ifFalse %s goto %s\n", condition, endLabel);
            free(condition);

            generateStatement(node->right);

            printf("goto %s\n", startLabel);
            printf("%s:\n", endLabel);

            free(startLabel);
            free(endLabel);
            break;
        }

        case NODE_BLOCK:
    enterTACScope();
    generateList(node->left);
    exitTACScope();
    break;
    }
}

static void clearTACVariables(void)
{
    while (variables != NULL) {
        Variable *temp = variables;
        variables = variables->next;

        free(temp->name);
        free(temp->tacName);
        free(temp);
    }

    currentScope = 0;
    scopeCount = 0;
    scopeDepth = 0;
}

void generateTAC(ASTNode *root)
{
    clearTACVariables();

    tempCount = 0;
    labelCount = 0;

    printf("\nThree Address Code:\n");

    if (root != NULL && root->type == NODE_PROGRAM)
        generateList(root->left);
    else
        generateList(root);

    clearTACVariables();
}

