
%{
#include <stdio.h>
#include <stdlib.h>

int yylex(void);
void yyerror(const char *s);
%}

%token INT FLOAT BOOL
%token IF ELSE WHILE PRINT
%token TRUE FALSE
%token IDENTIFIER
%token INTEGER_LITERAL FLOAT_LITERAL
%token AND OR EQ NE LE GE

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

%%

program:
    statements
    ;

statements:
    statements statement
    | statement
    ;

statement:
    declaration ';'
    | assignment ';'
    | print_statement ';'
    | if_statement
    | while_statement
    | block
    ;

declaration:
    type IDENTIFIER
    | type IDENTIFIER '=' expression
    ;

type:
    INT
    | FLOAT
    | BOOL
    ;

assignment:
    IDENTIFIER '=' expression
    ;

print_statement:
    PRINT '(' expression ')'
    ;

if_statement:
    IF '(' expression ')' statement %prec LOWER_THAN_ELSE
    | IF '(' expression ')' statement ELSE statement
    ;

while_statement:
    WHILE '(' expression ')' statement
    ;

block:
    '{' statements '}'
    ;

expression:
    INTEGER_LITERAL
    | FLOAT_LITERAL
    | TRUE
    | FALSE
    | IDENTIFIER
    | '(' expression ')'
    | '-' expression %prec UMINUS
    | '!' expression
    | expression '+' expression
    | expression '-' expression
    | expression '*' expression
    | expression '/' expression
    | expression '%' expression
    | expression '<' expression
    | expression '>' expression
    | expression LE expression
    | expression GE expression
    | expression EQ expression
    | expression NE expression
    | expression AND expression
    | expression OR expression
    ;

%%

void yyerror(const char *s)
{
    fprintf(stderr, "Syntax error: %s\n", s);
}

int main(void)
{
    printf("ParseFlow Compiler\n");
    printf("Enter your code:\n");

    if (yyparse() == 0)
        printf("\nSyntax analysis successful!\n");

    return 0;
}
