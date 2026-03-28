%{
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Enable float values in yacc
void yyerror(const char *s);
int yylex();
%}

%union {
    float fval;
}

// Tokens
%token <fval> NUMBER
%token '+' '-' '*' '/' '^' '(' ')'

// Data type for non-terminals
%type <fval> E T F G

// Operator precedence and associativity
%left '+' '-'
%left '*' '/'
%right '^'  // Right associative exponentiation
%left '(' ')'

%%

L   : E		{ printf("Result = %.0f\n", $1); }
    ;

// Grammar rules with SDD (Semantic Actions)
E   : E '+' T   { $$ = $1 + $3; }
    | E '-' T   { $$ = $1 - $3; }
    | T         { $$ = $1; }
    ;

T   : T '*' F   { $$ = $1 * $3; }
    | T '/' F   { $$ = $1 / $3; }
    | F         { $$ = $1; }
    ;

F   : G '^' F   { $$ = pow($1, $3); }  // Exponentiation handled separately
    | G         { $$ = $1; }
    ;

G   : '(' E ')' { $$ = $2; }
    | NUMBER    { $$ = $1; }
    ;

%%

// Error handling
void yyerror(const char *s) {
    printf("Error: %s\n", s);
}

// Main function
int main() {
    printf("Enter an arithmetic expression: ");
    yyparse();
    return 0;
}
