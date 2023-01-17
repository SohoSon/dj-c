/* pledge my Honor that I have not cheated,
 and will not cheat, on this assignment
-Xiaohu Sun */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "typecheck.h"

void printTCError(char *reason, int lineno) {
    printf("Semantic analysis error on line %d: \n", lineno);
    printf("    %s\n", reason);
    exit(-1);
}

void typecheckProgram()
{

    //1. calss name are unique
    // Array (symbol table) of class declarations
    // Note that the minimum array size is 1,
    //   due to the always-present Object class
    //int numClasses;  //size of the array
    //ClassDecl *classesST;  //the array itself
    if (numClasses >= 1) //at least there's one class
    {
        //printf("checking 1 \n");
        int a = 0;
            for(a = 0; a < numClasses; a++)
            {
                //printf("varName: %s \n", classesST[a].className);
                int b;
                for(b = a + 1; b < numClasses; b++) { 

                    //printf("varName2: %s \n", classesST[b].className);
                    int cmpClass = strcmp(classesST[a].className, classesST[b].className);
                    //printf("cmp: %d \n", cmpClass);
                    if(cmpClass == 0)   
                    {
                        //printf("dup found \n");
                        printTCError("Duplicated class name", classesST[b].classNameLineNumber);    
                    }
                }
            }
    }

    //2. CHECK all type in classesST and mainBlockST(?) are valid
    int a;
    for(a = 1; a < numClasses; a++)
    {   //a. checked
        //printf("checking 2a \n");
        if(classesST[a].superclass < 0)
        {
            printTCError("Invalid superclass: Super class lower than object", classesST[a].superclassLineNumber);
        }
        //b. checked
        //printf("checking 2b \n");
        int cmpsuperclass = strcmp(classesST[a].className, classesST[classesST[a].superclass].className);
        //printf("cmpsuperclass: %d \n", cmpsuperclass);
        if(cmpsuperclass == 0)
        {
            printTCError("Invalid superclass: Class type == Super type", classesST[a].classNameLineNumber);
        }
        //c. checked
        //printf("checking 2c \n");
        if (classesST[a].numVars > 0)
        {
            int t;
            for(t = 0; t<classesST[a].numVars; t++)
            {
                if(classesST[a].varList[t].type < -1)
                printTCError("Invalid variable type, var decl must equal or higher than nat", classesST[a].varList[t].typeLineNumber);

            //4. CHECK ALL field names are unique in defining class and superclass (checked)
                //printf("checking 4 \n");
                int tt;
                for(tt = t + 1; tt < classesST[a].numVars; tt++) { 
                    //printf("varName2: %s \n", mainBlockST[j].varName);
                    int cmpcs = strcmp(classesST[a].varList[t].varName, classesST[a].varList[tt].varName);
                    //printf("cmp: %d \n", cmp);
                    //if(mainBlockST[i].varName == mainBlockST[j].varName)   //dude, integer type safe??
                    if(cmpcs == 0)
                    {
                        //printf("dup found \n");
                        printTCError("Duplicated field variable name in class", classesST[a].varList[tt].varNameLineNumber);    
                    }
                }                
                if(classesST[classesST[a].superclass].numVars > 0) //super class's numVars > 0
                {
                    int sc;
                    for(sc = 0; sc < classesST[classesST[a].superclass].numVars; sc++) { 
                        //printf("varName2: %s \n", mainBlockST[j].varName);
                        int cmpcs1 = strcmp(classesST[a].varList[t].varName, classesST[classesST[a].superclass].varList[sc].varName);
                        //printf("cmp: %d \n", cmp);
                        //if(mainBlockST[i].varName == mainBlockST[j].varName)   //dude, integer type safe??
                        if(cmpcs1 == 0)
                        {
                            //printf("dup found \n");
                            printTCError("Variable declared multiple times (here and in a superclass)", classesST[a].varList[t].varNameLineNumber);    
                        }
                    }
                }


            }


        }
        //d. checked: method return and parameter types >= -1
        if (classesST[a].numMethods > 0)
        {
            //printf("checking 2d \n");
            int m;
            for(m = 0; m < classesST[a].numMethods; m++)
            {
                if(classesST[a].methodList[m].returnType < -1)
                printTCError("Invalid declared return type, method return type < -1", classesST[a].methodList[m].returnTypeLineNumber);
                if(classesST[a].methodList[m].paramType < -1)
                printTCError("Invalid declared parameter type, method parameter type < -1", classesST[a].methodList[m].paramTypeLineNumber);
                //e. local var in meth
                //printf("checking 2e \n");
                if(classesST[a].methodList[m].numLocals > 0) //might be empty
                {
                    int l;
                    for(l = 0; l < classesST[a].methodList[m].numLocals; l++)
                    {
                        if(classesST[a].methodList[m].localST[l].type < -1)
                        printTCError("method local variable type < -1", classesST[a].methodList[m].localST[l].typeLineNumber);
                    }
                }

            //5.
            //a.
                int mm;
                for(mm = m + 1; mm < classesST[a].numMethods; mm++) { 
                    //printf("varName2: %s \n", mainBlockST[j].varName);
                    int cmpmeth = strcmp(classesST[a].methodList[m].methodName, classesST[a].methodList[mm].methodName);
                    //printf("cmp: %d \n", cmp);
                    //if(mainBlockST[i].varName == mainBlockST[j].varName)   //dude, integer type safe??
                    if(cmpmeth == 0)
                    {
                        //printf("dup found \n");
                        printTCError("Duplicated method name in class", classesST[a].methodList[mm].methodNameLineNumber);    
                    }
                }
            //b. methods in superclasses with the same name has the same signature (para and ret)  CHECKED with bad42
                if(classesST[classesST[a].superclass].numMethods > 0) //super class's numVars > 0
                {
                    int mtsc;
                    for(mtsc = 0; mtsc < classesST[classesST[a].superclass].numMethods; mtsc++) { 
                        //printf("varName2: %s \n", mainBlockST[j].varName);
                        int cmpmethsc = strcmp(classesST[a].methodList[m].methodName, classesST[classesST[a].superclass].methodList[mtsc].methodName);
                        //printf("cmp: %d \n", cmp);
                        //if(mainBlockST[i].varName == mainBlockST[j].varName)   //dude, integer type safe??
                        if(cmpmethsc == 0)
                        {
                            //printf("dup found \n");
                            //checking signature (para and ret)
                            if(classesST[a].methodList[m].paramType != classesST[classesST[a].superclass].methodList[mtsc].paramType || classesST[a].methodList[m].returnType != classesST[classesST[a].superclass].methodList[mtsc].returnType)
                            printTCError("a superclass has a method with the same name but different signature (parameter/return type)", classesST[a].classNameLineNumber);    
                        }
                    }
                }
            //c. all para and local must has unique name
                if (classesST[a].methodList[m].numLocals > 0)
                {
                    //printf("checking 5c \n");
                    int c5 = 0;
                    for(c5 = 0; c5 < classesST[a].methodList[m].numLocals; c5++)
                    {
                        int cmpparalocal = strcmp(classesST[a].methodList[m].paramName, classesST[a].methodList[m].localST[c5].varName); //check para with all local
                        if(cmpparalocal == 0)
                        printTCError("Duplicated method local var name with para", classesST[a].methodList[m].paramNameLineNumber);
                        //printf("varName: %s \n", mainBlockST[i].varName);
                        int c5inner;
                        for(c5inner = c5 + 1; c5inner < classesST[a].methodList[m].numLocals; c5inner++) { 
                            //printf("varName2: %s \n", mainBlockST[j].varName);
                            int cmpmethlocal = strcmp(classesST[a].methodList[m].localST[c5].varName, classesST[a].methodList[m].localST[c5inner].varName);
                            //printf("cmp: %d \n", cmp);
                            //if(mainBlockST[i].varName == mainBlockST[j].varName)   //dude, integer type safe??
                            if(cmpmethlocal == 0)
                            {
                                //printf("dup found \n");
                                printTCError("Duplicated method local var name", classesST[a].methodList[m].localST[c5inner].varNameLineNumber);    
                            }
                        }
                    }
                }
                //d. exprList body is well typed AND it needs to be a subtype of methods decled ret type
                int methret = typeExprs(classesST[a].methodList[m].bodyExprs, a, m);
                //if (methret != classesST[a].methodList[m].returnType)
                if(!(isSubtype(methret, classesST[a].methodList[m].returnType)))
                printTCError("Method's declared and actual return types mismatch", classesST[a].methodList[m].returnTypeLineNumber);
            
            }

        }

            //3. Class hier is acyclic
        if(classesST[a].superclass != 0) //!= object
        {
            int tempSuper = classesST[a].superclass;
            while(classesST[tempSuper].superclass != 0) //current class: int: a
            {
                if(classesST[tempSuper].superclass == a)
                printTCError("Cyclic class declaration", classesST[tempSuper].superclassLineNumber);
                
                tempSuper = classesST[tempSuper].superclass;
            } 
        }
    }
        



    //6.check main block variable names are unique have no duplicates
    // Array (symbol table) of locals in the main block
    // int numMainBlockLocals;  size of the array
    // VarDecl *mainBlockST;  the array itself
    //Main block array might be 0!!!!!!!
    //(checked)
    if (numMainBlockLocals > 0)
    {
        //printf("checking 6 \n");
        int i = 0;
        for(i = 0; i < numMainBlockLocals; i++)
        {
            //printf("varName: %s \n", mainBlockST[i].varName);
            int j;
            for(j = i + 1; j < numMainBlockLocals; j++) { 
                //printf("varName2: %s \n", mainBlockST[j].varName);
                int cmp = strcmp(mainBlockST[i].varName, mainBlockST[j].varName);
                //printf("cmp: %d \n", cmp);
                //if(mainBlockST[i].varName == mainBlockST[j].varName)   //dude, integer type safe??
                if(cmp == 0)
                {
                    //printf("dup found \n");
                    printTCError("Duplicated variable name", mainBlockST[j].varNameLineNumber);    
                }
            }
        }
    }
    //7. CHECK the main block expr-list body is well typed (check every expr inthe list)
    //printf("checking 7 \n");
    int exprlistret = typeExprs(mainExprs, 0, 0);
}


int join(int t1, int t2) {
    if (isSubtype(t1, t2)) return t2;
    if (isSubtype(t2, t1)) return t1;
    return join(classesST[t1].superclass, t2);
}

/* HELPER METHODS FOR typecheckProgram(): */

/* Returns nonzero iff sub is a subtype of super */
int isSubtype(int sub, int super)
{   
    if(sub == super)
    return 1;
    if(sub == -2) //sub == null
    {
        if(super == -1)
        return 0;
        else if (super > -1)
        return 1;
    } 
    return 1;
    if ( sub == -1 || super == -1 )
    {
        return 0;
    } else {
        if (super == 0)
        {
            return 1;
        } else {
            int tempsub4super = sub;
            while(classesST[tempsub4super].superclass != 0)
            {
                if(classesST[tempsub4super].superclass == super)
                {
                    return 1;
                }
                tempsub4super = classesST[tempsub4super].superclass;
            }
            return 0;
        }
    }
}

/* Returns the type of the expression AST in the given context.
   Also sets t->staticClassNum and t->staticMemberNum attributes as needed.
   If classContainingExpr < 0 then this expression is in the main block of
   the program; otherwise the expression is in the given class. 
*/
int typeExpr(ASTree *t, int classContainingExpr, int methodContainingExpr){
    if (t == NULL) printTCError("type = null", t->lineNumber);
    if (t->typ == NAT_LITERAL_EXPR) 
    {
        return -1;
    }
    else if (t->typ == NOT_EXPR) 
    {
        //typeExpr(t->children->data, 0, 0);
        if(typeExpr(t->children->data, classContainingExpr, methodContainingExpr) != -1)
        printTCError("non-nat type in 'not' expression", t->lineNumber);
        else
        return -1;
    }
    else if (t->typ == PLUS_EXPR) {
        int expr1 = typeExpr(t->children->data, classContainingExpr, methodContainingExpr);
        int expr2 = typeExpr(t->children->next->data, classContainingExpr, methodContainingExpr);
        if (expr1 != -1 || expr2 != -1) {
            printTCError("non-nat type with + operator", t->lineNumber);
        } else {
            return expr1;
        }
    }
    else if (t->typ == MINUS_EXPR) {
        int expr1 = typeExpr(t->children->data, classContainingExpr, methodContainingExpr);
        int expr2 = typeExpr(t->children->next->data, classContainingExpr, methodContainingExpr);
        if (expr1 != -1 || expr2 != -1) {
            printTCError("non-nat type with - operator", t->lineNumber);
        } else {
            return expr1;
        }
    }
    else if (t->typ == TIMES_EXPR) {
        int expr1 = typeExpr(t->children->data, classContainingExpr, methodContainingExpr);
        int expr2 = typeExpr(t->children->next->data, classContainingExpr, methodContainingExpr);
        if (expr1 != -1 || expr2 != -1) {
            printTCError("non-nat type with * operator", t->lineNumber);
        } else {
            return expr1;
        }
    }
    else if (t->typ == EQUALITY_EXPR) {
        int expr1 = typeExpr(t->children->data, classContainingExpr, methodContainingExpr);
        int expr2 = typeExpr(t->children->next->data, classContainingExpr, methodContainingExpr);
        if (expr1 != expr2) {
            if (isSubtype(expr1, expr2) || isSubtype(expr2, expr1))
            return -1;
            else
            printTCError("Type mismatch with == operator", t->lineNumber);
        } else {
            return -1; //nat
        }
    }
    else if (t->typ == GREATER_THAN_EXPR) {
        int expr1 = typeExpr(t->children->data, classContainingExpr, methodContainingExpr);
        int expr2 = typeExpr(t->children->next->data, classContainingExpr, methodContainingExpr);
        if (expr1 == -1 && expr2 == expr1) {
            return expr1;
        } else {
            printTCError("non-nat type with > operator", t->lineNumber);
        }
    }
    else if (t->typ == OR_EXPR) {
        int expr1 = typeExpr(t->children->data, classContainingExpr, methodContainingExpr);
        int expr2 = typeExpr(t->children->next->data, classContainingExpr, methodContainingExpr);
        if (expr1 == -1 && expr2 == expr1) {
            return expr1;
        } else {
            printTCError("non-nat type in disjunction", t->lineNumber);
        }
    }
    else if (t->typ == NULL_EXPR) { return -2; }
    else if (t->typ == IF_THEN_ELSE_EXPR) {
        int expr1 = typeExpr(t->children->data, classContainingExpr, methodContainingExpr);
        int exprL1 = typeExprs(t->children->next->data, classContainingExpr, methodContainingExpr);
        int exprL2 = typeExprs(t->childrenTail->data, classContainingExpr, methodContainingExpr);
        if(expr1 == -1)
        {
            if(exprL1 == -1 && exprL1 == exprL2) //both nat
            {
                return -1; //nat type
            } 
            else if(exprL1 == exprL2) // both object
            {
                return exprL1;
            }
            else if(exprL1 != exprL2)
            {
                if(exprL1 == -1 || exprL2 == -1) //there nat
                printTCError("types of 'then' and 'else' branches mismatch", t->children->next->data->lineNumber);
                else
                return join(exprL1, exprL2);
            }
            else
            {
                printTCError("types of 'then' and 'else' branches mismatch", t->children->next->data->lineNumber);
            }
        }
        else 
        {
            printTCError("non-nat type in 'if' test", t->children->data->lineNumber);
        }
        
        //return expr1;
    }
    else if (t->typ == FOR_EXPR) {
        int expr1 = typeExpr(t->children->data, classContainingExpr, methodContainingExpr);
        int expr2 = typeExpr(t->children->next->data, classContainingExpr, methodContainingExpr);
        int expr3 = typeExpr(t->children->next->next->data, classContainingExpr, methodContainingExpr);
        int exprL = typeExprs(t->childrenTail->data, classContainingExpr, methodContainingExpr);
        if(expr2 == -1)
        return -1;
        else
        printTCError("non-nat type in loop test", t->lineNumber);
    }
    else if (t->typ == NEW_EXPR) { //id --- ID / OBJECT
        if(t->children->data->typ == AST_ID)
        {
            int i;
            for(i = 0; i < numClasses; i++)
            {
                //printf("class : %s \n" , classesST[i].className);
                //printf("new : %s \n" , t->children->data->idVal);
                int NewClassflag = strcmp(classesST[i].className, t->children->data->idVal);
                if(NewClassflag == 0)
                {
                    return i;
                }
            }
            printTCError("invalid new type, class doesn't exist", t->lineNumber);  
        }   
        else if (t->children->data->typ == OBJ_TYPE)         
        {
            return 0;
        } 
    }
    else if (t->typ == THIS_EXPR) {
        //printf("staticClassNum : %d \n" , t->staticClassNum);
        if(t->staticClassNum == 0)
        printTCError("This Keyword not in a class", t->lineNumber);

    }
    else if (t->typ == PRINT_EXPR) {
        int expr1 = typeExpr(t->children->data, classContainingExpr, methodContainingExpr);
        if (expr1 != -1)
        {
            printTCError("non-nat type in printNat", t->lineNumber);
        }
        else 
        {
            return expr1;
        }
    }
    else if (t->typ == READ_EXPR) {
        return -1;
    }
    
    else if (t->typ == ID_EXPR) { /* ID */
        char* id = t->children->data->idVal;
        int i;
        //printf("id_expr classnum: %d \n", classContainingExpr);
        if(classContainingExpr == 0) //1. main
        {
            for(i = 0; i < numMainBlockLocals; i++)
            {
                // printf("numMainBlockLocals: %d \n", numMainBlockLocals);
                // printf("TBL type: %s \n", mainBlockST[i].varName);
                // printf("ID type: %s \n", id);
                int IDflag = strcmp(mainBlockST[i].varName, id);
                if(IDflag == 0)
                {
                    return mainBlockST[i].type;
                }
            }
            printTCError("invalid type, type didn't decl", t->lineNumber);
        } 
        else    //2. current class
        {
            int j;
            for(i = 0; i < classesST[classContainingExpr].methodList[methodContainingExpr].numLocals; i++)        //
            {
                int cmpm = strcmp(id,classesST[classContainingExpr].methodList[methodContainingExpr].localST[i].varName);
                if(cmpm == 0)
                {
                    return classesST[classContainingExpr].methodList[methodContainingExpr].localST[i].type;
                }
            }
            int k;
            for(k = 0; k < classesST[classContainingExpr].numVars; k++)                                 //current class
            {
                int cmpmv = strcmp(id, classesST[classContainingExpr].varList[k].varName);
                if(cmpmv == 0)
                {
                    return classesST[classContainingExpr].varList[k].type;
                }
            }
            int cmpparam = strcmp(classesST[classContainingExpr].methodList[methodContainingExpr].paramName, id);
            if(cmpparam == 0)  //4.param
                return classesST[classContainingExpr].methodList[methodContainingExpr].paramType;
            printTCError("invalid type not found in class", t->lineNumber);
        }
    }
    else if (t->typ == DOT_ID_EXPR) { /* E.ID */
        int Etype = typeExpr(t->children->data, classContainingExpr, methodContainingExpr);
        char* id = t->children->next->data->idVal; //AST_ID? id_expr?
        if(Etype <= 0)
        printTCError("invalid class type, cant E.ID", t->lineNumber);
        else
        {
            int i;
            for(i = 1; i < numClasses; i++)
            {
                if(isSubtype(Etype, i)) //no dup in super so go all over it
                {
                    int j;
                    for(j = 0; j < classesST[i].numVars; j++)
                    {
                        int cmp = strcmp(classesST[i].varList[j].varName, id);
                        if(cmp == 0)
                        return classesST[i].varList[j].type;
                    }
                }
            }
            printTCError("no such var, cant E.ID", t->lineNumber);
        }
    } 
    else if (t->typ == ASSIGN_EXPR) { /* ID = E */
        int expr1 = typeExpr(t->children->data, classContainingExpr, methodContainingExpr);
        int expr2 = typeExpr(t->children->next->data, classContainingExpr, methodContainingExpr);

        if(isSubtype(expr2,expr1))
        return expr1;
        else
        printTCError("RHS is not subtype of LHS", t->lineNumber);
    }
    else if (t->typ == DOT_ASSIGN_EXPR) { /* E.ID = E */
        int Etype = typeExpr(t->children->data, classContainingExpr, methodContainingExpr);
        int IDtype;
        char* id = t->children->next->data->idVal; //AST_ID? id_expr?
        int expr2 = typeExpr(t->childrenTail->data, classContainingExpr, methodContainingExpr);
        
        if(Etype <= 0)
        printTCError("invalid class type, cant E.ID = E", t->lineNumber);
        else
        {
            int i;
            for(i = 1; i < numClasses; i++)
            {
                if(isSubtype(Etype, i)) //no dup in super so go all over it
                {
                    int j;
                    for(j = 0; j < classesST[i].numVars; j++)
                    {
                        int cmp = strcmp(classesST[i].varList[j].varName, id);
                        if(cmp == 0)
                        IDtype = classesST[i].varList[j].type;
                    }
                }
            }
            printTCError("no such var, cant E.ID = E", t->lineNumber);
        }

        if(isSubtype(expr2, IDtype))
        return IDtype;
        else
        printTCError("RHS is not subtype of LHS E.ID", t->lineNumber);
    }
    else if (t->typ == METHOD_CALL_EXPR) { /* ID(E) */
        char* id = t->children->data->idVal;
        int typepara = typeExpr(t->childrenTail->data, classContainingExpr, methodContainingExpr);

        int tempclass = classContainingExpr;
        int i;
        for(i = 0; i < classesST[tempclass].numMethods; i++)   //seacher itself and it's super class 
        {
            int cmp = strcmp(classesST[tempclass].methodList[i].methodName, id); 
            if(cmp == 0 && classesST[tempclass].methodList[i].paramType == typepara)
            return classesST[tempclass].methodList[i].returnType;
        }
        //superclass?
        //printTCError("ERROR: method not found", t->lineNumber);
        tempclass = classesST[classContainingExpr].superclass;
        while(tempclass != 0)
        {
            int j;
            for(j = 0; j < classesST[tempclass].numMethods; j++)   //seacher itself and it's super class 
            {
                int cmp = strcmp(classesST[tempclass].methodList[j].methodName, id); 
                if(cmp == 0 && isSubtype(typepara, classesST[tempclass].methodList[j].paramType))
                return classesST[tempclass].methodList[j].returnType;
            }
            tempclass = classesST[classContainingExpr].superclass;
        }
        printTCError("ERROR: method not found (including super class)", t->lineNumber);

    }
    else if (t->typ == DOT_METHOD_CALL_EXPR) { /* E.ID(E) */
        //printf("here /* E.ID(E) */ \n");
        char* id = t->children->next->data->idVal;  //ID
        int class = typeExpr(t->children->data, classContainingExpr, methodContainingExpr);
        //int class;
        //int typepara;
        int typepara = typeExpr(t->childrenTail->data, classContainingExpr, methodContainingExpr);
        if(class <= 0)
            printTCError("invalid class type, cant E.ID(E)", t->lineNumber);
        //find class of id
        int i;
        for(i = 1; i < numClasses; i++)
        {
            int j;
            if(isSubtype(class, i))
            {
                for(j = 0; j < classesST[i].numMethods; j++)
                {
                    int cmp = strcmp(classesST[i].methodList[j].methodName, id);
                    // if(cmp == 0)
                    // {
                    //     //printf("i, j = %d, %d \n", i , j);
                    //     //class = typeExpr(t->children->data, i, j);
                    //     //printf("class = %d \n", class);
                    //     if(class <= 0)
                    //         printTCError("invalid class type, cant E.ID(E)", t->lineNumber);
                    //     //typepara = typeExpr(t->childrenTail->data, i, j);
                    // }
                    if(cmp == 0 && isSubtype(typepara, classesST[i].methodList[j].paramType)) //sub of taget tpye: can pass to it
                    return classesST[i].methodList[j].returnType;
                }
            }
        }
        printTCError("ERROR: no such method, cant E.ID(E)", t->lineNumber);
    }
}

/* Returns the type of the EXPR_LIST AST in the given context. */
int typeExprs(ASTree *t, int classContainingExprs, int methodContainingExprs){
    if (t == NULL) printTCError("Error on ASTtree passed to tpyeExprs", t->lineNumber);
    ASTList *childrenHead = t->children;
    int exprHead = typeExpr(t->children->data, classContainingExprs, methodContainingExprs);
    while(childrenHead->next != NULL)
    {
        int expr1 = typeExpr(childrenHead->next->data, classContainingExprs, methodContainingExprs);
        childrenHead = childrenHead->next;
    }
    int ret = typeExpr(t->childrenTail->data, classContainingExprs, methodContainingExprs);
    return ret;
}
