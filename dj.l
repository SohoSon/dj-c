/* pledge my Honor that I have not cheated,
 and will not cheat, on this assignment
-Xiaohu Sun */

%option yylineno
%option noyywrap

%{
  #define DEBUG 0
  typedef YYTOKENTYPE Token;
  Token scanned(Token t);
%}

label	"//"
Identifiers [a-zA-Z][a-zA-Z0-9]*
natLiterals [0-9]+
wspace	[ \t\r\n]+

%%



class		{return scanned(CLASS);}
extends		{return scanned(EXTENDS);}
main		{return scanned(MAIN);}
nat		{return scanned(NATTYPE);}
Object		{return scanned(OBJECT);}
{natLiterals}		{return scanned(NATLITERAL);}
printNat		{return scanned(PRINTNAT);}
readNat		{return scanned(READNAT);}
if		{return scanned(IF);}
else		{return scanned(ELSE);}
for		{return scanned(FOR);}
null		{return scanned(NUL);}
new		{return scanned(NEW);}
this		{return scanned(THIS);}
{Identifiers}		{return scanned(ID);} 

"+"		{return scanned(PLUS);}
"-"		{return scanned(MINUS);} 
"*"		{return scanned(TIMES);}
"=="		{return scanned(EQUALITY);}
">"		{return scanned(GREATER);}
"||"		{return scanned(OR);}
"!"		{return scanned(NOT);}
"="		{return scanned(ASSIGN);}
"."		{return scanned(DOT);}
";"		{return scanned(SEMICOLON);}
"{"		{return scanned(LBRACE);} 
"}"		{return scanned(RBRACE);}
"("		{return scanned(LPAREN);}
")"		{return scanned(RPAREN);}
{label}   { char c = input();
             while(c!='\n' && c!=EOF) c=input();
	   }
{wspace}   { /*skip whitespace*/ }
<<EOF>>		{return scanned(ENDOFFILE);}
.          { if(DEBUG) printf("\n");
             printf("Lex error on line %d: Illegal character %s\n",
                    yylineno,yytext);
             exit(-1);
           }
%%

Token scanned(Token t) {
  if(DEBUG==0) return t;
  switch(t) {
    case CLASS: printf("CLASS "); return t;
    case ID: printf("ID(%s) ", yytext); return t;
    case EXTENDS: printf("EXTENDS "); return t;
    case MAIN: printf("MAIN "); return t;
    case NATTYPE: printf("NATTYPE "); return t;
    case OBJECT: printf("OBJECT "); return t;
    case NATLITERAL: printf("NATLITERAL(%s) ", yytext); return t;
    case PRINTNAT: printf("PRINTNAT "); return t;
    case READNAT: printf("READNAT "); return t;
    case IF: printf("IF "); return t;
    case ELSE: printf("ELSE "); return t;
    case FOR: printf("FOR "); return t;
    case PLUS: printf("PLUS "); return t;
    case MINUS: printf("MINUS "); return t;
    case TIMES: printf("TIMES "); return t;
    case EQUALITY: printf("EQUALITY "); return t;
  
    case GREATER: printf("GREATER "); return t;
    case OR: printf("OR "); return t;
    case NOT: printf("NOT "); return t;
    case ASSIGN: printf("ASSIGN "); return t;
    case NUL: printf("NUL "); return t;
    case NEW: printf("NEW "); return t;
    case THIS: printf("THIS "); return t;
    case DOT: printf("DOT "); return t;
    case SEMICOLON: printf("SEMICOLON "); return t;
    case LBRACE: printf("LBRACE "); return t;
    case RBRACE: printf("RBRACE "); return t;
    case LPAREN: printf("LPAREN "); return t;
    case RPAREN: printf("RPAREN "); return t;
    case ENDOFFILE: printf("ENDOFFILE\n"); return t;
    default: printf("ERROR: invalid token in scanned().\n"); exit(-1);
  }
}