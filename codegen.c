/* pledge my Honor that I have not cheated,
 and will not cheat, on this assignment
-Xiaohu Sun */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "typecheck.h"
#include "symtbl.h"

// void printTCError(char *reason, int lineno) {
//     printf("Semantic analysis error on line %d: \n", lineno);
//     printf("    %s\n", reason);
//     exit(-1);
// }

// LEVEL 1 : NO CLASSES , NO MAIN LOCALS:::: So, main has 

#define MAX_DISM_ADDR 65535 
 
/* Global for the DISM output file */ 
FILE *fout; 
//ASTree *expList;
int addrOffset;

/* Global to remember the next unique label number to use */ 
unsigned int labelNumber = 0; 
 
/* Declare mutually recursive functions (defs and docs appear below) */ 
void codeGenExpr(ASTree *t, int classNumber, int methodNumber); 
void codeGenExprs(ASTree *expList, int classNumber, int methodNumber); 
 
/* Print a message and exit under an exceptional condition */ 
void internalCGerror(char *msg) {
    printf("Error: %s\n", msg);
    exit(-1);
}
 
/*  Using  the  global  classesST,  calculate  the  total  number  of  fields,  
    including  inherited  fields,  in  an  object  of  the  given  type  */ 
int getNumObjectFields(int type) {
    if (type == 0)
        return 0;
    else if (type < 0)
        internalCGerror("Wrong call getNumObjectFields: This type shouldn't have a field");
    else 
        return getNumObjectFields(classesST[type].superclass) + classesST[type].numMethods;
}
 
/* Generate code that increments the stack pointer */ 
void incSP() {
    fprintf(fout, "       mov 1 1 \n");       
    fprintf(fout, "       add 6 6 1  ; SP++\n");    
}
 
/* Generate code that decrements the stack pointer */ 
void decSP() {
    int checkSP = labelNumber++;
    fprintf(fout, "       mov 1 1 \n");
    fprintf(fout, "       sub 6 6 1  ; SP--\n");    
    fprintf(fout, "       bgt 6 5 #%d ; branch if SP>HP\n", checkSP);
    fprintf(fout, "       mov 1 77 ; error code 77 => out of stack memory\n");
    fprintf(fout, "       hlt 1  ; out of stack memory! (SP <= HP) \n");
    fprintf(fout, "#%d: mov 0 0\n", checkSP); //skip halt    
}
 
/* Output code to check for a null value at the top of the stack. 
   If the top stack value (at M[SP+1]) is null (0), the DISM code 
   output will halt. */ 
// void checkNullDereference() {

// }
 
/* Generate DISM code for the given single expression, which appears 
   in the given class and method (or main block). 
   If classNumber < 0 then methodNumber may be anything and we assume  
   we are generating code for the program's main block. */ 
void codeGenExpr(ASTree *t, int classNumber, int methodNumber) {
    if (t == NULL) {
        internalCGerror("type is null");
    };
    if (t->typ == NAT_LITERAL_EXPR) 
    {
        fprintf(fout, "       mov 1 %d \n", t->natVal);
        fprintf(fout, "       str 6 0 1  ; M[SP] <- R[r1] (a nat literal)\n");
        decSP();
    }
    else if (t->typ == NOT_EXPR) 
    {
        int Equalzero = labelNumber++;
        int keepGoing = labelNumber++;
        codeGenExpr(t->children->data, classNumber, methodNumber);
        fprintf(fout, "       lod 1 6 1  ; R[r1] <- M[SP + 1] (lod res.(nat) to r1)\n");
        fprintf(fout, "       beq 1 0 #Equalzero%d  ; if R[1] == 0 then PC <- n\n", Equalzero); //check if it's 0
        fprintf(fout, "       str 6 1 0  ; M[SP + 1] <- R[0] (store 0 to 1st opd SP)\n"); // if not equal to 0, set it 0
        fprintf(fout, "       jmp 0 #keepGoing%d \n", keepGoing); // finish this expr
        fprintf(fout, "#Equalzero%d: mov 1 1  ; r1 = 1 \n", Equalzero);
        fprintf(fout, "       str 6 1 1  ; M[SP + 1] <- R[1] (store 1 to 1st opd SP)\n");
        fprintf(fout, "#keepGoing%d: mov 0 0  ; no-op\n", keepGoing);
        //incSP(); //nothing need to keep in Stack
    }
    else if (t->typ == PLUS_EXPR) {
        codeGenExpr(t->children->data, classNumber, methodNumber);
        codeGenExpr(t->children->next->data, classNumber, methodNumber);
        fprintf(fout, "       lod 1 6 2  ; R[r1] <- M[SP + 2] (1st operand)\n");
        fprintf(fout, "       lod 2 6 1  ; R[r2] <- M[SP + 1] (2nd operand)\n");
        fprintf(fout, "       add 1 1 2  ; R[r1] <- R[r1] + R[r2]\n");
        fprintf(fout, "       str 6 2 1  ; M[SP + 2] <- R[r1] (sotre sum to 1st SP)\n");
        incSP();
    }
    else if (t->typ == MINUS_EXPR) {
        codeGenExpr(t->children->data, classNumber, methodNumber);
        codeGenExpr(t->children->next->data, classNumber, methodNumber);
        fprintf(fout, "       lod 1 6 2  ; R[r1] <- M[SP + 2] (1st operand)\n");
        fprintf(fout, "       lod 2 6 1  ; R[r2] <- M[SP + 1] (2nd operand)\n");
        fprintf(fout, "       sub 1 1 2  ; R[r1] <- R[r1] - R[r2]\n");
        fprintf(fout, "       str 6 2 1  ; M[SP + 2] <- R[r1] (sotre sub res. to 1st SP)\n");
        incSP();
    }
    else if (t->typ == TIMES_EXPR) {
        codeGenExpr(t->children->data, classNumber, methodNumber);
        codeGenExpr(t->children->next->data, classNumber, methodNumber);
        fprintf(fout, "       lod 1 6 2  ; R[r1] <- M[SP + 2] (1st operand)\n");
        fprintf(fout, "       lod 2 6 1  ; R[r2] <- M[SP + 1] (2nd operand)\n");
        fprintf(fout, "       mul 1 1 2  ; R[r1] <- R[r1] * R[r2]\n");
        fprintf(fout, "       str 6 2 1  ; M[SP + 2] <- R[r1] (sotre mul res. to 1st SP)\n");
        incSP();
    }
    else if (t->typ == EQUALITY_EXPR) {
        int EQUALITY = labelNumber++;
        //int notEqual = labelNumber++;
        int keepGoing = labelNumber++;
        codeGenExpr(t->children->data, classNumber, methodNumber);
        codeGenExpr(t->children->next->data, classNumber, methodNumber);
        fprintf(fout, "       lod 1 6 2  ; R[r1] <- M[SP + 2] (1st operand)\n");
        fprintf(fout, "       lod 2 6 1  ; R[r2] <- M[SP + 1] (2nd operand)\n");
        fprintf(fout, "       beq 1 2 #EQUALITY%d  ; R[r1] = R[r2] then PC <- n\n", EQUALITY); //not equal going down
        fprintf(fout, "       str 6 2 0  ; M[SP + 2] <- R[0] (store 0 to 1st opd SP)\n"); // not equal str 0
        fprintf(fout, "       jmp 0 #keepGoing%d \n", keepGoing); // finish this expr
        fprintf(fout, "#EQUALITY%d: mov 1 1  ; r1 = 1 \n", EQUALITY);
        fprintf(fout, "       str 6 2 1  ; M[SP + 2] <- R[1] (store 1 to 1st opd SP)\n");
        fprintf(fout, "#keepGoing%d: mov 0 0  ; no-op\n", keepGoing);
        incSP(); //clear 2nd SP
    }
    else if (t->typ == GREATER_THAN_EXPR) {
        int grtr = labelNumber++;
        //int lessNequal = labelNumber++;
        int keepGoing = labelNumber++;
        codeGenExpr(t->children->data, classNumber, methodNumber);
        codeGenExpr(t->children->next->data, classNumber, methodNumber);
        fprintf(fout, "       lod 1 6 2  ; R[r1] <- M[SP + 2] (1st operand)\n");
        fprintf(fout, "       lod 2 6 1  ; R[r2] <- M[SP + 1] (2nd operand)\n");
        fprintf(fout, "       bgt 1 2 #grtr%d  ; R[r1] > R[r2] then PC <- n\n", grtr); //lessNequal going down
        fprintf(fout, "       str 6 2 0  ; M[SP + 2] <- R[0] (store 0 to 1st opd SP)\n"); //lessNequal str 0
        fprintf(fout, "       jmp 0 #keepGoing%d \n", keepGoing); //jmp to finish
        fprintf(fout, "#grtr%d: mov 1 1  ; r1 = 1 \n", grtr);
        fprintf(fout, "       str 6 2 1  ; M[SP + 2] <- R[1] (store 1 to 1st opd SP)\n");
        fprintf(fout, "#keepGoing%d: mov 0 0  ; no-op\n", keepGoing);
        incSP();
    }
    else if (t->typ == OR_EXPR) {
        int trueOR = labelNumber++;
        //int falseOR = labelNumber++;
        int keepGoing = labelNumber++;
        codeGenExpr(t->children->data, classNumber, methodNumber);
        codeGenExpr(t->children->next->data, classNumber, methodNumber);
        fprintf(fout, "       mov 1 1  ; r1 = 1 \n");
        fprintf(fout, "       lod 2 6 2  ; R[r2] <- M[SP + 2] (1st operand into r2)\n");
        fprintf(fout, "       beq 1 2 #trueOR%d  ;  e1 == 1 branch to true\n", trueOR); //e1 == 1 branch to true
        fprintf(fout, "       lod 2 6 1  ; R[r2] <- M[SP + 1] (2nd operand)\n");
        fprintf(fout, "       beq 1 2 #trueOR%d  ; e2 == 1 branch to true\n", trueOR); //e2 == 1 branch to true
        fprintf(fout, "       str 6 2 0  ; M[SP + 2] <- R[0] (FALSE store 0 to 1st opd SP)\n");  //if e1 e2 all false, str 0
        fprintf(fout, "       jmp 0 #keepGoing%d  ; all e1 e2 == 0 branch to false \n", keepGoing); // skip str 1
        fprintf(fout, "#trueOR%d: str 6 2 1  ; M[SP + 2] <- R[1] (TRUE store 1 to 1st opd SP)\n", trueOR); // str 1
        fprintf(fout, "#keepGoing%d: mov 0 0  ; no-op\n", keepGoing);
        incSP();
    }
    else if (t->typ == NULL_EXPR) {
        fprintf(fout, "       str 6 0 0  ; M[SP] <- 0 (null)\n");
        decSP();
    }
    else if (t->typ == IF_THEN_ELSE_EXPR) {
        int elselable = labelNumber++;
        int endlabel = labelNumber++;
        codeGenExpr(t->children->data, classNumber, methodNumber);
        fprintf(fout, "       lod 1 6 1  ; R[r1] <- M[SP + 1] (load test res. into r1) \n");
        fprintf(fout, "       beq 0 1 #elselable%d  ; skip to else \n", elselable);
        incSP(); //clear e1 res. in if(e1)
        codeGenExprs(t->children->next->data,  classNumber, methodNumber);
        fprintf(fout, "       jmp 0 #endlabel%d  ; skip else branch \n", endlabel);
        fprintf(fout, "#elselable%d: mov 0 0  ; begin else branch \n", elselable);
        incSP(); //clear e1 res. in if(e1)
        codeGenExprs(t->childrenTail->data,  classNumber, methodNumber);
        fprintf(fout, "#endlabel%d: mov 0 0  ; end if-then-else expr \n", endlabel);
    }
    else if (t->typ == FOR_EXPR) { //LVL1 rn
        int looplable = labelNumber++;
        int endlabel = labelNumber++;
        int loopbacklable = labelNumber++;
        codeGenExpr(t->children->data, classNumber, methodNumber); //e1
        fprintf(fout, "#%d: mov 0 0  ; one iter ends: loop back here\n", loopbacklable);
        codeGenExpr(t->children->next->data, classNumber, methodNumber); //e2 : condition
        //fprintf(fout, "       lod 1 6 2  ; R[r1] <- M[SP + 2] (1st operand)\n"); //exe once
        fprintf(fout, "       lod 1 6 1  ; R[r2] <- M[SP + 1] (2nd operand: condition in for loop)\n");
        fprintf(fout, "       beq 0 1 #%d  ; skip to end \n", endlabel);

        codeGenExprs(t->childrenTail->data, classNumber, methodNumber); //el1   
        incSP(); //in loop
        incSP(); 
        incSP(); 
        codeGenExpr(t->children->next->next->data, classNumber, methodNumber); //e3 SP+2
        //fprintf(fout, "       lod 1 6 1  ; R[r1] <- M[SP + 1] (3rd operand)\n"); //re-work every iter
        fprintf(fout, "       jmp 0 #%d  ; loop end, skip next iter\n", loopbacklable);
        fprintf(fout, "#%d: mov 0 0  ; end for-loop expr\n", endlabel);
    }
    else if (t->typ == NEW_EXPR) { /*id --- ID / OBJECT */ 
        int checkHP = labelNumber++;
        int endlabel = labelNumber++;
        int loop = labelNumber++;
        int n; // number of fields
        int type;
        if(t->children->data->typ == OBJ_TYPE) // OBJECT
            type = 0;
        else //ID
            type = classNameToNumber(t->children->data->idVal);
        n = getNumObjectFields(type);
        // check (HP+n+1) < SP
        fprintf(fout, "       mov 1 1  ; NOW checking heap for NEW_expr\n");
        fprintf(fout, "       mov 2 %d  ; this is number of fields\n", n);
        fprintf(fout, "       add 1 2 1  ; n + 1\n");    
        fprintf(fout, "       add 1 5 1  ; n + 1 + HP\n");    
        fprintf(fout, "       bgt 6 5 #%d ; branch if SP > HP + n + 1\n", checkHP);
        fprintf(fout, "       mov 1 66 ; error code 66 => out of HEAP memory\n");
        fprintf(fout, "       hlt 1  ; out of HEAP memory! (HP + n + 1 >= SP) \n");
        fprintf(fout, "#%d: mov 0 0  ; HP memo ok!\n", checkHP); //skip halt
        fprintf(fout, "       mov 1 1  ; NOW implementing heap for NEW_expr\n");
        //Repeat n times
        fprintf(fout, "#loop%d: mov 0 0  ; n loop here \n", loop);       
        fprintf(fout, "       beq 2 0 #end%d  ; if n == 0  \n", endlabel);       
        fprintf(fout, "       str 5 0 0  ; M[R[5] + 0] <- R[0] \n");
        fprintf(fout, "       add 5 5 1  ; HP++ increament heap \n");
        fprintf(fout, "       sub 2 2 1  ; n-- \n");
        fprintf(fout, "       jmp 0 #loop%d  ; jump back begin of loop \n", loop);
        fprintf(fout, "#end%d: mov 0 0  ; n loop here \n", endlabel);
        fprintf(fout, "       mov 2 %d  ; \n", type);
        fprintf(fout, "       str 5 0 2  ; store type num in HP - 1\n");
        fprintf(fout, "       str 6 0 5  ; str HP-1 in stack\n");
        fprintf(fout, "       add 5 5 1  ; HP++ \n");
        decSP();
    }
    else if (t->typ == THIS_EXPR) { }
    else if (t->typ == PRINT_EXPR) {
        codeGenExpr(t->children->data, classNumber, methodNumber);
        fprintf(fout, "       lod 1 6 1  ; R[r1] <- M[SP + 1] (lod res.(nat) to r1)\n");
        fprintf(fout, "       ptn 1  ; Print natural number R[s] to screen\n");
        //incSP(); //nothing need to keep in Stack
    }
    else if (t->typ == READ_EXPR) {
        fprintf(fout, "       rdn 1  ; Read natural number from screen into R[d]\n");
        fprintf(fout, "       str 6 0 1  ; M[SP + 0] <- R[r1] (store readnat to SP)\n");
        decSP();
    }
    
    else if (t->typ == ID_EXPR) { /* ID */ 
        char* id = t->children->data->idVal;
        int i;
        int addr;
        if(classNumber < 0) //if it's in main
        {
            for(i = 0; i < numMainBlockLocals; i++)
            {              
                int IDflag = strcmp(mainBlockST[i].varName, id);
                if(IDflag == 0){
                    addr = MAX_DISM_ADDR - i;
                    if(mainBlockST[i].type == -1)
                    {
                        fprintf(fout, "       mov 1 %d  ; ID_EXPR: this is calculated NAT addr\n", addr);
                        fprintf(fout, "       lod 2 1 0  ; R[r2] <- M[SP + 1] (lod nat val from above addr into r2)\n"); ////here
                        fprintf(fout, "       str 6 0 2  ; M[SP] <- R[r1] (str this nat val to SP)\n");
                    } else {
                        fprintf(fout, "       mov 1 %d  ; ID_EXPR: calc addr\n", addr);
                        fprintf(fout, "       str 6 0 1  ; M[SP] <- R[r1] (ID_EXPR calc addr)\n");
                    }
                    decSP();
                    return;
                }
            }
        }  
    }
    else if (t->typ == DOT_ID_EXPR) {
        codeGenExpr(t->children->data,classNumber,methodNumber);
        char* name = t->children->next->data->idVal;
        int varIndex = getNumObjectFields(*staticClass) - getFieldOffset(name,*staticClass,staticClass);
        fprintf(fout,"mov 1 %d;Id expression\n",varIndex);
        char* code = "lod 2 6 1\n"
                     "sub 1 2 1\n"
                     "lod 1 1 0\n"
                     "str 6 1 1\n";
        fprintf(fout,"%s",code);
        incSP();
        checkNullDereference();
        decSP(); /* E.ID */
    } 
    else if (t->typ == ASSIGN_EXPR) { /* ID = E */
        int addr1; //left: pass to!
        //int addr2; //right: load from
        int type;
        char* id = t->children->data->idVal;
        //fprintf(fout, "here's id: %s\n", id);
        int i;
        //fprintf(fout, "here's classNumber: %d\n", classNumber);
        if(classNumber < 0) //if it's in main
        {
            for(i = 0; i < numMainBlockLocals; i++)
            {              
                int IDflag = strcmp(mainBlockST[i].varName, id);
                //fprintf(fout, "here's tbl %s\n", mainBlockST[i].varName);
                //fprintf(fout, "here's num %d\n", numMainBlockLocals);
                if(IDflag == 0){
                    //fprintf(fout, "here's id %d\n", IDflag);
                    type = mainBlockST[i].type;
                    addr1 = MAX_DISM_ADDR - i;
                }
            }
        }
        //AST_ID addr1
        codeGenExpr(t->children->next->data, classNumber, methodNumber);
        //addr2 = addrOffset;
        if(type != -1){
            fprintf(fout, "       mov 1 %d  ; lhs address pass to\n", addr1);
            fprintf(fout, "       lod 2 6 1  ; R[r2] <- M[SP + 1] (lod rhs addr)\n");
            fprintf(fout, "       lod 3 2 0  ; R[r3] <- M[rhs addr + 0] (value)\n");
            fprintf(fout, "       str 1 0 3  ; M[lhs addr + 0] <- R[r3] (sotre value to lhs addr)\n");
        } else {
            fprintf(fout, "       mov 1 %d  ; lhs address pass to\n", addr1);
            fprintf(fout, "       lod 2 6 1  ; R[r2] <- M[SP + 1] (lod nat val)\n");
            fprintf(fout, "       str 1 0 2  ; M[lhs addr + 0] <- R[r3] (sotre value to lhs addr)\n");
        }
        //incSP(); //because it's assign to local? 

    }
    else if (t->typ == DOT_ASSIGN_EXPR) {
        codeGenExpr(t->children->next->next->data,classNumber,methodNumber); 
        codeGenExpr(t->children->data,classNumber,methodNumber);
        char* name = t->children->next->data->idVal;
        int varIndex = getNumObjectFields(*staticClass) - getFieldOffset(name,*staticClass,staticClass);
        fprintf(fout,"mov 1 %d;Id expression\n",varIndex);
        char* code = "lod 2 6 1\n"
                     "sub 1 2 1\n"
                     "lod 2 6 2\n"
                     "ptn 1\n"
                     "str 1 0 2\n";
        fprintf(fout,"%s",code);
        incSP();
         /* E.ID = E */ 
    }
    else if (t->typ == METHOD_CALL_EXPR) {
         /* ID(E) */}
    else if (t->typ == DOT_METHOD_CALL_EXPR) {
         /* E.ID(E) */ }
}

/* Generate DISM code for an expression list, which appears in 
     the given class and method (or main block). 
   If classNumber < 0 then methodNumber may be anything and we assume  
   we are generating code for the program's main block. */ 
void codeGenExprs(ASTree *expList, int classNumber, int methodNumber){
    if (expList == NULL) {
        internalCGerror("Error on ASTtree passed to codeGenExprs");
    }
    
    ASTList *childrenHead = expList->children;
    //int exprHead = typeExpr(expList->children->data, classContainingExprs, methodContainingExprs);
    codeGenExpr(expList->children->data, classNumber, methodNumber);
    if(childrenHead != expList->childrenTail)
    {
        //fprintf(fout, "i'm not the tail\n");
        incSP();
    }

    while(childrenHead->next != NULL)
    {
        //int expr1 = typeExpr(childrenHead->next->data, classContainingExprs, methodContainingExprs);
        codeGenExpr(childrenHead->next->data, classNumber, methodNumber);
        if(childrenHead->next != expList->childrenTail){
            //fprintf(fout, "i'm not the tail\n");
            incSP();
        }
            
        childrenHead = childrenHead->next;
    }
    //int ret = typeExpr(t->childrenTail->data, classContainingExprs, methodContainingExprs);
    //decSP();
}
 
/* Generate DISM code as the prologue to the given method or main 
   block.  If classNumber < 0 then methodNumber may be anything and we 
   assume we are generating code for the program's main block. */ 
void genPrologue(int classNumber, int methodNumber) {
    //1. Init FP, SP, HP
    fprintf(fout, "       mov 7 65535  ; initialize FP\n");
    fprintf(fout, "       mov 6 65535  ; initialize SP\n");
    fprintf(fout, "       mov 5 1  ; initialize HP\n");
    //2. Allocate space of main's locals ----NOT FOR LVL1
    fprintf(fout, "       mov 0 0  ; ALLOCATE STACK SPACE FOR MAIN LOCALS\n");

    if(classNumber < 0)
    {
        if( numMainBlockLocals > 0 ){
            int i;
            for(i = 0; i < numMainBlockLocals; i++)
            decSP();
            // fprintf(fout, "       mov 1 %d  ; mov numMainBlockLocals into r1 \n", numMainBlockLocals);
            // fprintf(fout, "       sub 6 6 1  ; SP <- SP - numMainBlockLocals \n"); // ALLOCATE STACK SPACE FOR MAIN LOCALS
            //fprintf(fout, "ptn 6 ; SP <- SP - numMainBlockLocals \n");
        }
    }
}
 
/* Generate DISM code as the epilogue to the given method or main  
   block.  If classNumber < 0 then methodNumber may be anything and we 
   assume we are generating code for the program's main block. */ 
void genEpilogue(int classNumber, int methodNumber) {
    fprintf(fout, "       hlt 0  ; NORMAL TERMINATION AT END OF MAIN BLOCK\n");
}
 
/* Generate DISM code for the given method or main block. 
   If classNumber < 0 then methodNumber may be anything and we assume 
   we are generating code for the program's main block. */ 
void genBody(int classNumber, int methodNumber) {
    fprintf(fout, "       mov 0 0  ; BEGIN METHOD/MAIN-BLOCK BODY\n");
    codeGenExprs(mainExprs, classNumber, methodNumber);
}
 
/* Map a given (1) static class number, (2) a method number defined 
   in that class, and (3) a dynamic object's type to: 
   (a) the dynamic class number and (b) the dynamic method number that 
   actually get called when an object of type (3) dynamically invokes 
   method (2).   
   This method assumes that dynamicType is a subtype of staticClass. */ 
// void getDynamicMethodInfo(int staticClass, int staticMethod,  
//    int dynamicType, int *dynamicClassToCall, int *dynamicMethodToCall) {

//    }
 
/* Emit code for the program's vtable, beginning at label #VTABLE. 
   The vtable jumps (i.e., dispatches) to code based on 
   (1) the dynamic calling object's address (at M[SP+4]), 
   (2) the calling object's static type (at M[SP+3]), and 
   (3) the static method number (at M[SP+2]). */ 
// void genVTable() {

// }

void generateDISM(FILE *outputFile){
    //1. genPrologue 
    fout = outputFile;
    genPrologue(-1, -1);
    genBody(-1, -1);
    genEpilogue(-1, -1);
}