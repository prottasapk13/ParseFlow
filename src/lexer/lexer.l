
%option noyywrap yylineno

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

{FLOATNUM}          { yylval.str = strdup(yytext); return FLOAT_LITERAL; }
{NUMBER}            { yylval.str = strdup(yytext); return INTEGER_LITERAL; }
{ID}                { yylval.str = strdup(yytext); return IDENTIFIER; }

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
