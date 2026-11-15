
%option noyywrap yylineno

%{
#include <stdio.h>
#include "parser.tab.h"
%}

DIGIT       [0-9]
LETTER      [a-zA-Z_]
ID          {LETTER}({LETTER}|{DIGIT})*
NUMBER      {DIGIT}+
FLOATNUM    {DIGIT}+"."{DIGIT}+

%%

"int"               { return INT; }
"float"             { return FLOAT; }
"bool"              { return BOOL; }
"if"                { return IF; }
"else"              { return ELSE; }
"while"             { return WHILE; }
"print"             { return PRINT; }
"true"              { return TRUE; }
"false"             { return FALSE; }

"&&"                { return AND; }
"||"                { return OR; }
"=="                { return EQ; }
"!="                { return NE; }
"<="                { return LE; }
">="                { return GE; }

{FLOATNUM}          { return FLOAT_LITERAL; }
{NUMBER}            { return INTEGER_LITERAL; }
{ID}                { return IDENTIFIER; }

"+"                 { return '+'; }
"-"                 { return '-'; }
"*"                 { return '*'; }
"/"                 { return '/'; }
"%"                 { return '%'; }
"<"                 { return '<'; }
">"                 { return '>'; }
"!"                 { return '!'; }
"="                 { return '='; }

";"                 { return ';'; }
","                 { return ','; }
"("                 { return '('; }
")"                 { return ')'; }
"{"                 { return '{'; }
"}"                 { return '}'; }

[ \t\r]+            ;
\n                  ;

"//".*              ;

.                   { printf("Lexical error at line %d: %s\n", yylineno, yytext); }

%%
