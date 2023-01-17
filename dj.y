/* pledge my Honor that I have not cheated,
 and will not cheat, on this assignment
-Xiaohu Sun */

/* DJ PARSER */

%code provides {
  #include "lex.yy.c"
  #include <stdlib.h>
  #include "ast.h"

  /* Symbols in this grammar are represented as ASTs */ 
  #define YYSTYPE ASTree *

  /* Declare global AST for entire program */
  ASTree *pgmAST;

  /* Function for printing generic syntax-error messages */
  void yyerror(const char *str) {
    printf("Syntax error on line %d at token %s\n",yylineno,yytext);
    printf("(This version of the compiler exits after finding the first ");
    printf("syntax error.)\n");
    exit(-1);
  }
}

%token CLASS ID EXTENDS MAIN NATTYPE OBJECT
%token NATLITERAL PRINTNAT READNAT IF ELSE FOR
%token PLUS MINUS TIMES EQUALITY GREATER OR NOT
%token ASSIGN NUL NEW THIS DOT SEMICOLON
%token LBRACE RBRACE LPAREN RPAREN ENDOFFILE

%start pgm

%right ASSIGN
%left OR
%nonassoc EQUALITY GREATER
%left PLUS MINUS
%left TIMES
%right NOT

/*
%nonassoc NEW
%nonassoc THIS

%left LBRACE RBRACE
%left LPAREN RPAREN 
*/
%left DOT


%%

pgm : classDlist MAIN LBRACE variableDlist exprlist RBRACE ENDOFFILE
      { 
        pgmAST = newAST(PROGRAM,$1,0,NULL,yylineno); 
        appendToChildrenList(pgmAST,$4);
        appendToChildrenList(pgmAST,$5);
        return 0; 
      }
    | classDlist MAIN LBRACE exprlist RBRACE ENDOFFILE
      { 
        pgmAST = newAST(PROGRAM,$1,0,NULL,yylineno); 
        appendToChildrenList(pgmAST,newAST(VAR_DECL_LIST,NULL,0,NULL,yylineno));
        appendToChildrenList(pgmAST,$4);
        return 0; 
      }
    | MAIN LBRACE variableDlist exprlist RBRACE ENDOFFILE
      { 
        pgmAST = newAST(PROGRAM,NULL,0,NULL,yylineno); 
        appendToChildrenList(pgmAST,newAST(CLASS_DECL_LIST,NULL,0,NULL,yylineno));
        appendToChildrenList(pgmAST,$3);
        appendToChildrenList(pgmAST,$4);
        return 0; 
      }
    | MAIN LBRACE exprlist RBRACE ENDOFFILE
      { 
        pgmAST = newAST(PROGRAM,NULL,0,NULL,yylineno);
        appendToChildrenList(pgmAST,newAST(CLASS_DECL_LIST,NULL,0,NULL,yylineno));
        appendToChildrenList(pgmAST,newAST(VAR_DECL_LIST,NULL,0,NULL,yylineno));
        appendToChildrenList(pgmAST,$3); 
        return 0; 
      }
    ;



/* --- FINISHED POSSIBLE EMPTY */
classDlist : classDlist classD
           {  
             appendToChildrenList($1,$2);
           }
           | classD
           { $$ = newAST(CLASS_DECL_LIST,$1,0,NULL,yylineno); }
           ;

/* --- FINISHED */
classD : CLASS id EXTENDS id LBRACE variableDlist methodDlist RBRACE
       { $$ = newAST(CLASS_DECL,$2,0,NULL,yylineno); 
         appendToChildrenList($$,$4);
         appendToChildrenList($$,$6);
         appendToChildrenList($$,$7);
       }
       | CLASS id EXTENDS id LBRACE methodDlist RBRACE
       { $$ = newAST(CLASS_DECL,$2,0,NULL,yylineno); 
         appendToChildrenList($$,$4);
         appendToChildrenList($$,newAST(VAR_DECL_LIST,NULL,0,NULL,yylineno));
         appendToChildrenList($$,$6);
       }
       | CLASS id EXTENDS id LBRACE RBRACE
       { $$ = newAST(CLASS_DECL,$2,0,NULL,yylineno); 
         appendToChildrenList($$,$4);
         appendToChildrenList($$,newAST(VAR_DECL_LIST,NULL,0,NULL,yylineno));
         appendToChildrenList($$,newAST(METHOD_DECL_LIST,NULL,0,NULL,yylineno));
       }
       | CLASS id EXTENDS id LBRACE variableDlist RBRACE
       { $$ = newAST(CLASS_DECL,$2,0,NULL,yylineno); 
         appendToChildrenList($$,$4);
         appendToChildrenList($$,$6);
         appendToChildrenList($$,newAST(METHOD_DECL_LIST,NULL,0,NULL,yylineno));
       }
       ;



/* --- FINISHED POSSIBLE EMPTY */
variableDlist : variableDlist variableD
              {  
                appendToChildrenList($1,$2);
              }
              | variableD
              { $$ = newAST(VAR_DECL_LIST,$1,0,NULL,yylineno); }
              ;

/* --- FINISHED obj VD*/
variableD : natt id SEMICOLON
          { $$ = newAST(VAR_DECL,$1,0,NULL,yylineno); 
              appendToChildrenList($$,$2);
          }
          | id id SEMICOLON
          { $$ = newAST(VAR_DECL,$1,0,NULL,yylineno); 
              appendToChildrenList($$,$2);
          }
          ;

/* --- FINISHED */
methodDlist : methodDlist methodD
            {  
              appendToChildrenList($1,$2);
            }
            | methodD
            { $$ = newAST(METHOD_DECL_LIST,$1,0,NULL,yylineno); }
            ;

/* --- FINISHED obj MD*/
methodD : natt id LPAREN natt id RPAREN LBRACE variableDlist exprlist RBRACE
        { $$ = newAST(METHOD_DECL,$1,0,NULL,yylineno); 
          appendToChildrenList($$,$2);
          appendToChildrenList($$,$4);
          appendToChildrenList($$,$5);
          appendToChildrenList($$,$8);
          appendToChildrenList($$,$9);
        }
        | natt id LPAREN natt id RPAREN LBRACE exprlist RBRACE
        { $$ = newAST(METHOD_DECL,$1,0,NULL,yylineno); 
          appendToChildrenList($$,$2);
          appendToChildrenList($$,$4);
          appendToChildrenList($$,$5);
          appendToChildrenList($$,newAST(VAR_DECL_LIST,NULL,0,NULL,yylineno));
          appendToChildrenList($$,$8);
        }
        | natt id LPAREN id id RPAREN LBRACE variableDlist exprlist RBRACE
        { $$ = newAST(METHOD_DECL,$1,0,NULL,yylineno); 
          appendToChildrenList($$,$2);
          appendToChildrenList($$,$4);
          appendToChildrenList($$,$5);
          appendToChildrenList($$,$8);
          appendToChildrenList($$,$9);
        }
        | natt id LPAREN id id RPAREN LBRACE exprlist RBRACE
        { $$ = newAST(METHOD_DECL,$1,0,NULL,yylineno); 
          appendToChildrenList($$,$2);
          appendToChildrenList($$,$4);
          appendToChildrenList($$,$5);
          appendToChildrenList($$,newAST(VAR_DECL_LIST,NULL,0,NULL,yylineno));
          appendToChildrenList($$,$8);
        }
        | id id LPAREN natt id RPAREN LBRACE variableDlist exprlist RBRACE
        { $$ = newAST(METHOD_DECL,$1,0,NULL,yylineno); 
          appendToChildrenList($$,$2);
          appendToChildrenList($$,$4);
          appendToChildrenList($$,$5);
          appendToChildrenList($$,$8);
          appendToChildrenList($$,$9);
        }
        | id id LPAREN natt id RPAREN LBRACE exprlist RBRACE
        { $$ = newAST(METHOD_DECL,$1,0,NULL,yylineno); 
          appendToChildrenList($$,$2);
          appendToChildrenList($$,$4);
          appendToChildrenList($$,$5);
          appendToChildrenList($$,newAST(VAR_DECL_LIST,NULL,0,NULL,yylineno));
          appendToChildrenList($$,$8);
        }
        | id id LPAREN id id RPAREN LBRACE variableDlist exprlist RBRACE
        { $$ = newAST(METHOD_DECL,$1,0,NULL,yylineno); 
          appendToChildrenList($$,$2);
          appendToChildrenList($$,$4);
          appendToChildrenList($$,$5);
          appendToChildrenList($$,$8);
          appendToChildrenList($$,$9);
        }
        | id id LPAREN id id RPAREN LBRACE exprlist RBRACE
        { $$ = newAST(METHOD_DECL,$1,0,NULL,yylineno); 
          appendToChildrenList($$,$2);
          appendToChildrenList($$,$4);
          appendToChildrenList($$,$5);
          appendToChildrenList($$,newAST(VAR_DECL_LIST,NULL,0,NULL,yylineno));
          appendToChildrenList($$,$8);
        }
        ;        


/* FINISHED */
exprlist : exprlist expr SEMICOLON
         { 
            appendToChildrenList($1,$2);
         }
         | expr SEMICOLON
         { $$ = newAST(EXPR_LIST,$1,0,NULL,yylineno); }
         ;

/* FINISHED expr = new obj() */
expr : expr PLUS expr
     { $$ = newAST(PLUS_EXPR,$1,0,NULL,yylineno);
        appendToChildrenList($$,$3);
     }
     | expr MINUS expr
     { $$ = newAST(MINUS_EXPR,$1,0,NULL,yylineno);
       appendToChildrenList($$,$3);
     }
     | expr TIMES expr
     { $$ = newAST(TIMES_EXPR,$1,0,NULL,yylineno);
       appendToChildrenList($$,$3);
     }
     | expr EQUALITY expr
     { $$ = newAST(EQUALITY_EXPR,$1,0,NULL,yylineno);
       appendToChildrenList($$,$3);
     }
     | expr GREATER expr
     { $$ = newAST(GREATER_THAN_EXPR,$1,0,NULL,yylineno);
       appendToChildrenList($$,$3);
     }
     | NOT expr
     { $$ = newAST(NOT_EXPR,$2,0,NULL,yylineno); }
     | expr OR expr
     { $$ = newAST(OR_EXPR,$1,0,NULL,yylineno);
       appendToChildrenList($$,$3);
     }
     | NATLITERAL
     { $$ = newAST(NAT_LITERAL_EXPR,NULL,atoi(yytext),NULL,yylineno); }
     | NUL
     { $$ = newAST(NULL_EXPR,NULL,0,NULL,yylineno); }
     | IF LPAREN expr RPAREN LBRACE exprlist RBRACE ELSE LBRACE exprlist RBRACE
     { $$ = newAST(IF_THEN_ELSE_EXPR,$3,0,NULL,yylineno);
       appendToChildrenList($$,$6);
       appendToChildrenList($$,$10);
     }
     | FOR LPAREN expr SEMICOLON expr SEMICOLON expr RPAREN LBRACE exprlist RBRACE
     { $$ = newAST(FOR_EXPR,$3,0,NULL,yylineno);
       appendToChildrenList($$,$5);
       appendToChildrenList($$,$7);
       appendToChildrenList($$,$10);
     }
     | NEW id LPAREN RPAREN
     { $$ = newAST(NEW_EXPR,$2,0,NULL,yylineno); }
     | THIS
     { $$ = newAST(THIS_EXPR,NULL,0,NULL,yylineno); }
     | PRINTNAT LPAREN expr RPAREN
     { $$ = newAST(PRINT_EXPR,$3,0,NULL,yylineno); }
     | READNAT LPAREN RPAREN
     { $$ = newAST(READ_EXPR,NULL,0,NULL,yylineno); }


     | id                             /* ID */
     { $$ = newAST(ID_EXPR,$1,0,NULL,yylineno); }
     | expr DOT id                    /* E.ID */
     { $$ = newAST(DOT_ID_EXPR,$1,0,NULL,yylineno); 
       appendToChildrenList($$,$3);
     }
     | id ASSIGN expr                 /* ID = E */
     { $$ = newAST(ASSIGN_EXPR,$1,0,NULL,yylineno); 
       appendToChildrenList($$,$3);
     }
     | expr DOT id ASSIGN expr        /* E.ID = E */
     { $$ = newAST(DOT_ASSIGN_EXPR,$1,0,NULL,yylineno); 
       appendToChildrenList($$,$3);
       appendToChildrenList($$,$5);
     }
     | id LPAREN expr RPAREN          /* ID(E) */
     { $$ = newAST(METHOD_CALL_EXPR,$1,0,NULL,yylineno); 
       appendToChildrenList($$,$3);
     }
     | expr DOT id LPAREN expr RPAREN /* E.ID(E) */
     { $$ = newAST(DOT_METHOD_CALL_EXPR,$1,0,NULL,yylineno); 
       appendToChildrenList($$,$3);
       appendToChildrenList($$,$5);
     }
     | LPAREN expr RPAREN
     { $$ = $2; }
     ;

/* FINISHED FOR FIX Object in class */
id : ID
   { $$ = newAST(AST_ID,NULL,0,yytext,yylineno); }
   | OBJECT
   { $$ = newAST(OBJ_TYPE,NULL,0,NULL,yylineno); }
   ;
natt : NATTYPE
     { $$ = newAST(NAT_TYPE,NULL,0,NULL,yylineno); }
     ;


%%

int main(int argc, char **argv) {
  if(argc!=2) {
    printf("Usage: dj-parse filename\n");
    exit(-1);
  }
  yyin = fopen(argv[1],"r");
  if(yyin==NULL) {
    printf("ERROR: could not open file %s\n",argv[1]);
    exit(-1);
  }
  /* parse the input program */
  
  yyparse();
  printf("****** begin abstract syntax tree for DJ program ********\n");
  printAST(pgmAST);
  printf("******* end abstract syntax tree for DJ program *********\n\n");

  return 0;
}

