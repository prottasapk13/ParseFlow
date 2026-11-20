
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

ASTNode *createNode(NodeType type, const char *value,
                    ASTNode *left, ASTNode *right)
{
    ASTNode *node = malloc(sizeof(ASTNode));

    if (node == NULL) {
        printf("Memory allocation failed!\n");
        exit(1);
    }

    node->type = type;
    node->value = value ? strdup(value) : NULL;
    node->line_number = 0;
    node->left = left;
    node->right = right;
    node->next = NULL;

    return node;
}

void printAST(ASTNode *node, int level)
{
    while (node != NULL) {
        for (int i = 0; i < level; i++)
            printf("  ");

        switch (node->type) {
            case NODE_PROGRAM: printf("PROGRAM"); break;
            case NODE_DECLARATION: printf("DECLARATION"); break;
            case NODE_ASSIGNMENT: printf("ASSIGNMENT"); break;
            case NODE_PRINT: printf("PRINT"); break;
            case NODE_IF: printf("IF"); break;
            case NODE_WHILE: printf("WHILE"); break;
            case NODE_BLOCK: printf("BLOCK"); break;
            case NODE_BINARY: printf("BINARY"); break;
            case NODE_UNARY: printf("UNARY"); break;
            case NODE_IDENTIFIER: printf("IDENTIFIER"); break;
            case NODE_INTEGER: printf("INTEGER"); break;
            case NODE_FLOAT: printf("FLOAT"); break;
            case NODE_BOOLEAN: printf("BOOLEAN"); break;
        }

        if (node->value != NULL)
            printf(": %s", node->value);

        printf("\n");

        printAST(node->left, level + 1);
        printAST(node->right, level + 1);

        node = node->next;
    }
}

void freeAST(ASTNode *node)
{
    if (node == NULL)
        return;

    freeAST(node->left);
    freeAST(node->right);
    freeAST(node->next);

    free(node->value);
    free(node);
}
