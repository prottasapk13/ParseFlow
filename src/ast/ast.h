
#ifndef AST_H
#define AST_H

typedef enum {
    NODE_PROGRAM,
    NODE_DECLARATION,
    NODE_ASSIGNMENT,
    NODE_PRINT,
    NODE_IF,
    NODE_WHILE,
    NODE_BLOCK,
    NODE_BINARY,
    NODE_UNARY,
    NODE_IDENTIFIER,
    NODE_INTEGER,
    NODE_FLOAT,
    NODE_BOOLEAN
} NodeType;

typedef struct ASTNode {
    NodeType type;
    char *value;
    int line_number;

    struct ASTNode *left;
    struct ASTNode *right;
    struct ASTNode *next;

} ASTNode;

ASTNode *createNode(NodeType type, const char *value,
                    ASTNode *left, ASTNode *right);

void printAST(ASTNode *node, int level);

void freeAST(ASTNode *node);

#endif
