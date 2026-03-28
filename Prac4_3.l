%option noyywrap
%{
	#include<stdio.h>
	int c=0;
	int w=0;
	int l=1;
	int inword=0;
%}

%%
[ \t\n]	{ c++; 
	  if(inword){
		w++;
		inword=0;
	  }
	  if (yytext[0] == '\n') l++;
	}
.	{ c++; inword=1; }
%%
int main()
{
	FILE *file = fopen("input.txt", "r");

	yyin = file;
	yylex();

	if (inword) w++;

	printf("\nCharacters : %d",c);
	printf("\nWords : %d",w);
	printf("\nLines : %d",l);

	fclose(file);
	return 0;
}