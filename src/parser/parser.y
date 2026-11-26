
%code requires {
#include "../ast/ast.h"
}

%locations

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../ast/ast.h"
#include "../semantic/semantic.h"
#include "../tac/tac.h"

int yylex(void);
void yyerror(const char *s);

static ASTNode *appendNode(ASTNode *list, ASTNode *node)
{
    if (list == NULL)
        return node;

    ASTNode *temp = list;

    while (temp->next != NULL)
        temp = temp->next;

    temp->next = node;
    return list;
}

ASTNode *root = NULL;
%}

%union {
    char *str;
    ASTNode *node;
}

%token INT FLOAT BOOL
%token IF ELSE WHILE PRINT
%token TRUE FALSE
%token AND OR EQ NE LE GE

%token <str> IDENTIFIER INTEGER_LITERAL FLOAT_LITERAL

%type <str> type
%type <node> program optional_statements statements
%type <node> statement declaration assignment
%type <node> print_statement if_statement while_statement
%type <node> block expression

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

%left OR
%left AND
%left EQ NE
%left '<' '>' LE GE
%left '+' '-'
%left '*' '/' '%'
%right '!'
%right UMINUS

%start program

%%

program:
    optional_statements
    {
        root = createNode(NODE_PROGRAM, "Program", $1, NULL);
        $$ = root;
    }
    ;

optional_statements:
    statements
    {
        $$ = $1;
    }
    |
    {
        $$ = NULL;
    }
    ;

statements:
    statement
    {
        $$ = $1;
    }
    |
    statements statement
    {
        $$ = appendNode($1, $2);
    }
    ;

statement:
    declaration ';'       { $$ = $1; }
    | assignment ';'      { $$ = $1; }
    | print_statement ';' { $$ = $1; }
    | if_statement        { $$ = $1; }
    | while_statement    { $$ = $1; }
    | block               { $$ = $1; }
    ;


declaration:
    type IDENTIFIER
    {
        $$ = createNode(NODE_DECLARATION, $1,
             createNode(NODE_IDENTIFIER, $2, NULL, NULL), NULL);

        $$->line_number = @2.first_line;

        free($1);
        free($2);
    }
    |
    type IDENTIFIER '=' expression
    {
        $$ = createNode(NODE_DECLARATION, $1,
             createNode(NODE_IDENTIFIER, $2, NULL, NULL), $4);

        $$->line_number = @2.first_line;

        free($1);
        free($2);
    }
    ;

type:
    INT   { $$ = strdup("int"); }
    | FLOAT { $$ = strdup("float"); }
    | BOOL  { $$ = strdup("bool"); }
    ;


assignment:
    IDENTIFIER '=' expression
    {
        $$ = createNode(NODE_ASSIGNMENT, "=",
             createNode(NODE_IDENTIFIER, $1, NULL, NULL), $3);

        $$->line_number = @1.first_line;
        $$->left->line_number = @1.first_line;

        free($1);
    }
    ;

print_statement:
    PRINT '(' expression ')'
    {
        $$ = createNode(NODE_PRINT, "print", $3, NULL);
        $$->line_number = @1.first_line;
    }
    |
    PRINT IDENTIFIER
    {
        $$ = createNode(NODE_PRINT, "print",
             createNode(NODE_IDENTIFIER, $2, NULL, NULL), NULL);

        $$->line_number = @1.first_line;
        $$->left->line_number = @2.first_line;

        free($2);
    }
    ;
     
if_statement:
    IF '(' expression ')' statement %prec LOWER_THAN_ELSE
    {
        $$ = createNode(NODE_IF, "if", $3, $5);
        $$->line_number = @1.first_line;
    }
    |
    IF '(' expression ')' statement ELSE statement
    {
        $5->next = $7;
        $$ = createNode(NODE_IF, "if-else", $3, $5);
        $$->line_number = @1.first_line;
    }
    ;

while_statement:
    WHILE '(' expression ')' statement
    {
        $$ = createNode(NODE_WHILE, "while", $3, $5);
        $$->line_number = @1.first_line;
    }
    ;

    

block:
    '{' optional_statements '}'
    {
        $$ = createNode(NODE_BLOCK, "block", $2, NULL);
    }
    ;
   
	
expression:
    INTEGER_LITERAL
    {
        $$ = createNode(NODE_INTEGER, $1, NULL, NULL);
        $$->line_number = @1.first_line;
        free($1);
    }
    |
    FLOAT_LITERAL
    {
        $$ = createNode(NODE_FLOAT, $1, NULL, NULL);
        $$->line_number = @1.first_line;
        free($1);
    }
    |
    TRUE
    {
        $$ = createNode(NODE_BOOLEAN, "true", NULL, NULL);
        $$->line_number = @1.first_line;
    }
    |
    FALSE
    {
        $$ = createNode(NODE_BOOLEAN, "false", NULL, NULL);
        $$->line_number = @1.first_line;
    }
    |
    IDENTIFIER
    {
        $$ = createNode(NODE_IDENTIFIER, $1, NULL, NULL);
        $$->line_number = @1.first_line;
        free($1);
    }
    |
    '(' expression ')'
    {
        $$ = $2;
    }
    |
    '-' expression %prec UMINUS
    {
        $$ = createNode(NODE_UNARY, "-", $2, NULL);
        $$->line_number = @1.first_line;
    }
    |
    '!' expression
    {
        $$ = createNode(NODE_UNARY, "!", $2, NULL);
        $$->line_number = @1.first_line;
    }
    |
    expression '+' expression
    {
        $$ = createNode(NODE_BINARY, "+", $1, $3);
        $$->line_number = @2.first_line;
    }
    |
    expression '-' expression
    {
        $$ = createNode(NODE_BINARY, "-", $1, $3);
        $$->line_number = @2.first_line;
    }
    |
    expression '*' expression
    {
        $$ = createNode(NODE_BINARY, "*", $1, $3);
        $$->line_number = @2.first_line;
    }
    |
    expression '/' expression
    {
        $$ = createNode(NODE_BINARY, "/", $1, $3);
        $$->line_number = @2.first_line;
    }
    |
    expression '<' expression
    {
        $$ = createNode(NODE_BINARY, "<", $1, $3);
        $$->line_number = @2.first_line;
    }
    |
    expression '>' expression
    {
        $$ = createNode(NODE_BINARY, ">", $1, $3);
        $$->line_number = @2.first_line;
    }
    |
    expression LE expression
    {
        $$ = createNode(NODE_BINARY, "<=", $1, $3);
        $$->line_number = @2.first_line;
    }
    |
    expression GE expression
    {
        $$ = createNode(NODE_BINARY, ">=", $1, $3);
        $$->line_number = @2.first_line;
    }
    |
    expression EQ expression
    {
        $$ = createNode(NODE_BINARY, "==", $1, $3);
        $$->line_number = @2.first_line;
    }
    |
    expression NE expression
    {
        $$ = createNode(NODE_BINARY, "!=", $1, $3);
        $$->line_number = @2.first_line;
    }
    |
    expression AND expression
    {
        $$ = createNode(NODE_BINARY, "&&", $1, $3);
        $$->line_number = @2.first_line;
    }
    |
    expression OR expression
    {
        $$ = createNode(NODE_BINARY, "||", $1, $3);
        $$->line_number = @2.first_line;
    }
    ;


%%

void yyerror(const char *s)
{
    fprintf(stderr, "Syntax Error at line %d: %s\n",
            yylloc.first_line, s);
}

int main(void)
{
    printf("ParseFlow Compiler\n");
    printf("Enter your code:\n\n");

    if (yyparse() == 0) {
        printf("\nSyntax analysis successful!\n");
        printf("\nAbstract Syntax Tree:\n");
        printAST(root, 0);

        int errors = analyzeSemantics(root);

if (errors == 0) {
    generateTAC(root);
} else {
    printf("\nTAC generation skipped due to semantic errors.\n");
}
	
    }

    freeAST(root);
    return 0;
}
