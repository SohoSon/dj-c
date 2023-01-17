# dj-c

Compilation of the Compiler 
Use the following commands to compile the complete djc from scratch: 
''''
> flex dj.l 
> bison -v dj.y 
> sed -i '/extern YYSTYPE yylval/d' dj.tab.c 
> gcc dj.tab.c ast.c symtbl.c typecheck.c codegen.c -o djc
''''

Example Executions 
Your  compiler  should  continue  to  report  lexical,  syntactic,  and  semantic  errors  in  the 
source  program  before  exiting.    As  with  previous  assignments,  you  need  not  report 
multiple  errors.    If  there  are  no  lexical,  syntactic,  or  semantic  errors,  your  compiler 
generates DISM code. 
 
Your compiler should enforce that the given source-program filename ends in “.dj”: > ./djc 
Usage: djc file.dj 
> ./djc good99.dj 
ERROR: could not open file good99.dj 
> ./djc good1.dism 
Error: Input filename does not match *.dj

The compiler produces no output (besides the target-code file) when compiling a valid DJ 
program: > ./djc good1.dj 
> 
After compiling good1.dj, we can find the djc-generated code in file good1.dism:        mov 7 65535  ; initialize FP 
       mov 6 65535  ; initialize SP 
       mov 5 1  ; initialize HP 
       mov 0 0  ; ALLOCATE STACK SPACE FOR MAIN LOCALS 
       mov 0 0  ; BEGIN METHOD/MAIN-BLOCK BODY 
       mov 1 0 
       str 6 0 1  ; M[SP] <- R[r1] (a nat literal) 
       mov 1 1 
       sub 6 6 1  ; SP-- 
       bgt 6 5 #0  ; branch if SP>HP 
       mov 1 77 ; error code 77 => out of stack memory 
       hlt 1  ; out of stack memory! (SP <= HP) 
#0: mov 0 0 
       hlt 0  ; NORMAL TERMINATION AT END OF MAIN BLOCK
       
       
