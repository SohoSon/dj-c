/* File codegen.h: Header File for Code Generator for djc */

#ifndef CODEGEN_H 
#define CODEGEN_H

#include <stdio.h>

/* Perform code generation for the compiler's input program.
   The code generation is based on the enhanced symbol tables built
   in setupSymbolTables, which is declared in symtbl.h.

   This method writes DISM code for the whole program to the 
   specified outputFile (which must be open and ready for writes 
   before calling generateDISM).

   This method assumes that setupSymbolTables(), declared in 
   symtbl.h, and typecheckProgram(), declared in typecheck.h, 
   have already executed.
*/
void generateDISM(FILE *outputFile);

#define MAX_DISM_ADDR 65535 
 
/* Global for the DISM output file */ 
FILE *fout; 
 
/* Global to remember the next unique label number to use */ 
unsigned int labelNumber = 0; 
 
/* Declare mutually recursive functions (defs and docs appear below) */ 
void codeGenExpr(ASTree *t, int classNumber, int methodNumber); 
void codeGenExprs(ASTree *expList, int classNumber, int methodNumber); 
 
/* Print a message and exit under an exceptional condition */ 
void internalCGerror(char *msg) 
 
/*  Using  the  global  classesST,  calculate  the  total  number  of  fields,  
    including  inherited  fields,  in  an  object  of  the  given  type  */ 
int getNumObjectFields(int type) 
 
/* Generate code that increments the stack pointer */ 
void incSP() 
 
/* Generate code that decrements the stack pointer */ 
void decSP() 
 
/* Output code to check for a null value at the top of the stack. 
   If the top stack value (at M[SP+1]) is null (0), the DISM code 
   output will halt. */ 
void checkNullDereference() 
 
/* Generate DISM code for the given single expression, which appears 
   in the given class and method (or main block). 
   If classNumber < 0 then methodNumber may be anything and we assume  
   we are generating code for the program's main block. */ 
void codeGenExpr(ASTree *t, int classNumber, int methodNumber) 
 
 
/* Generate DISM code for an expression list, which appears in 
     the given class and method (or main block). 
   If classNumber < 0 then methodNumber may be anything and we assume  
 
3 
   we are generating code for the program's main block. */ 
void codeGenExprs(ASTree *expList, int classNumber, int methodNumber) 
 
/* Generate DISM code as the prologue to the given method or main 
   block.  If classNumber < 0 then methodNumber may be anything and we 
   assume we are generating code for the program's main block. */ 
void genPrologue(int classNumber, int methodNumber) 
 
/* Generate DISM code as the epilogue to the given method or main  
   block.  If classNumber < 0 then methodNumber may be anything and we 
   assume we are generating code for the program's main block. */ 
void genEpilogue(int classNumber, int methodNumber)  
 
/* Generate DISM code for the given method or main block. 
   If classNumber < 0 then methodNumber may be anything and we assume 
   we are generating code for the program's main block. */ 
void genBody(int classNumber, int methodNumber) 
 
/* Map a given (1) static class number, (2) a method number defined 
   in that class, and (3) a dynamic object's type to: 
   (a) the dynamic class number and (b) the dynamic method number that 
   actually get called when an object of type (3) dynamically invokes 
   method (2).   
   This method assumes that dynamicType is a subtype of staticClass. */ 
void getDynamicMethodInfo(int staticClass, int staticMethod,  
   int dynamicType, int *dynamicClassToCall, int *dynamicMethodToCall) 
 
/* Emit code for the program's vtable, beginning at label #VTABLE. 
   The vtable jumps (i.e., dispatches) to code based on 
   (1) the dynamic calling object's address (at M[SP+4]), 
   (2) the calling object's static type (at M[SP+3]), and 
   (3) the static method number (at M[SP+2]). */ 
void genVTable()



#endif

