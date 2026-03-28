%option noyywrap
%{
#include <stdio.h>
#include <string.h>

int lineNo = 1;

char errors[200][100];
int errLine[200];
int errCount = 0;

char symbolTable[100][50];
int symCount = 0;

void addError(const char *text)
{
    errLine[errCount] = lineNo;
    strcpy(errors[errCount++], text);
}

void addSymbol(char *id)
{
    int i;
    for(i=0;i<symCount;i++)
        if(strcmp(symbolTable[i],id)==0)
            return;

    strcpy(symbolTable[symCount++],id);
}
%}

/* ---------- DEFINITIONS ---------- */

KEYWORD     int|float|double|char|if|else|while|for|return|void|break|continue|long|short|signed|unsigned|switch|case|struct
ID          [a-zA-Z_][a-zA-Z0-9_]*
NUM         ([0-9]+(\.[0-9]+)?)|(\.[0-9]+)
CHARCONST   \'(.)\'
STRING      \"([^\"\n]*)\"
OP          [+\-*/%=<>!&|.]
PUNC        [;,(){}\[\]]

%%

"//".*                      ;   /* ignore single-line comment */

"/*"([^*]|\*+[^*/])*\*+"/" ;   /* ignore multi-line comment */

[ \t\r]+                   ;   /* ignore whitespace */

\n                         { lineNo++; }

{KEYWORD}                  { printf("Keyword: %s\n", yytext); }

{ID}                       { printf("Identifier: %s\n", yytext);
                             addSymbol(yytext); }

{NUM}                      { printf("Constant: %s\n", yytext); }

{CHARCONST}                { printf("Constant: %s\n", yytext); }

{STRING}                   { printf("Constant: %s\n", yytext); }

{OP}                       { printf("Operator: %s\n", yytext); }

{PUNC}                     { printf("Punctuation: %s\n", yytext); }

[0-9]+[a-zA-Z]+            { addError(yytext); }

.                          { addError(yytext); }

%%

int main()
{
    FILE *fp = fopen("input.txt", "r");

    if(fp == NULL)
    {
        printf("Cannot open input.txt\n");
        return 1;
    }

    yyin = fp;   // tell lexer to read from file

    printf("TOKENS\n");
    yylex();

    fclose(fp);

    printf("\nLEXICAL ERRORS\n");
    int i;
    for(i=0;i<errCount;i++)
	printf("Line %d : %s invalid lexeme\n", errLine[i], errors[i]);

    printf("\nSYMBOL TABLE ENTRIES\n");
    for(i=0;i<symCount;i++)
        printf("%d) %s\n", i+1, symbolTable[i]);

    printf("\n\n23CE093 - Dhruv Patel\n");
    return 0;
}