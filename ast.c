/* pledge my Honor that I have not cheated,
 and will not cheat, on this assignment
-Xiaohu Sun */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ast.h"

void printError(char *reason) {
  printf("AST Error: %s\n", reason);
  exit(-1);
}

ASTree *newAST(ASTNodeType t, ASTree *child, unsigned int natAttribute, 
        char *idAttribute, unsigned int lineNum) {

  ASTree *toReturn = malloc(sizeof(ASTree));
  if(toReturn==NULL) printError("malloc in newAST()");
  toReturn->typ = t;
  toReturn->lineNumber = lineNum;

  

  // That is, create a one-node list of children for the new tree.
  ASTList *childList = malloc(sizeof(ASTList));   //A linked list for List of Nodes(data: ASTree; next: NULL)
  if(childList==NULL) printError("malloc in newAST()");

  if(child == NULL)
  {
    childList->data = NULL;
    childList->next = NULL;
  } else {
    childList->data = child;
    childList->next = NULL;
  }

  if(t == NAT_LITERAL_EXPR)
  {
    toReturn->children = childList;
    toReturn->childrenTail = childList;
    //attri:
    toReturn->natVal = natAttribute;
  }
  else if(t == AST_ID)
  {
    toReturn->children = childList;
    toReturn->childrenTail = childList;
    //attri:
    char *copyStr = malloc(strlen(idAttribute)+1);
    if(copyStr==NULL) printError("malloc in newAST()");
      strcpy(copyStr, idAttribute);
    toReturn->idVal = copyStr;
  } else {
    toReturn->children = childList;
    toReturn->childrenTail = childList;
    //attri:
    toReturn->natVal = natAttribute;
    if(idAttribute==NULL)
      toReturn->idVal=NULL;
    else {
      char *copyStr1 = malloc(strlen(idAttribute)+1);
      if(copyStr1==NULL) 
        printError("malloc in newAST()");
      strcpy(copyStr1, idAttribute);
      toReturn->idVal = copyStr1;
    }
  }
  return toReturn;
}


/* Create a new AST node of type t */
// ASTree *newAST(ASTNodeType t, ASTree *child1,
//         ASTree *child2, ASTree *child3, char *attribute) {
//   ASTree *toReturn = malloc(sizeof(ASTree));
//   if(toReturn==NULL) printError("malloc in newAST()");
//   toReturn->typ = t;
//   // create a linked list of children
//   ASTList *childList = malloc(sizeof(ASTList));
//   if(childList==NULL) printError("malloc in newAST()");
//   childList->data = child1;
//   childList->next = NULL;
//   toReturn->childrenHead = childList;
//   toReturn->childrenTail = childList;
//   if(child2!=NULL) appendAST(toReturn, child2);
//   if(child3!=NULL) appendAST(toReturn, child3);
//   // finally set this node's attribute
//   if(attribute==NULL) toReturn->text=NULL;
//   else {
//     char *copyStr = malloc(strlen(attribute)+1);
//     if(copyStr==NULL) printError("malloc in newAST()");
//     strcpy(copyStr, attribute);
//     toReturn->text = copyStr;
//   }
//   return toReturn;
// }

void appendToChildrenList(ASTree *parent, ASTree *newChild) {
  if(parent==NULL) printError("append called with null parent");
  if(parent->children==NULL || parent->childrenTail==NULL)
    printError("append called with bad parent");
  if(newChild==NULL) printError("append called with null newChild");
  if(parent->childrenTail->data==NULL) //replace empty tail with new child
    parent->childrenTail->data = newChild;
  else {
    ASTList *newList = malloc(sizeof(ASTList));
    if(newList==NULL) printError("malloc in appendAST()");
    newList->data = newChild;
    newList->next = NULL;
    parent->childrenTail->next = newList;
    parent->childrenTail = newList;
  }
}

// /* Append a new child AST node onto a parent's list of children */
// ASTree *appendAST(ASTree *parent, ASTree *newChild) {
//   if(parent==NULL) printError("append called with null parent");
//   if(parent->childrenHead==NULL || parent->childrenTail==NULL)
//     printError("append called with bad parent");
//   if(newChild==NULL) printError("append called with null newChild");
//   if(parent->childrenTail->data==NULL) //replace empty tail with new child
//     parent->childrenTail->data = newChild;
//   else { //tail of children is currently nonempty; append new child to list
//     ASTList *newList = malloc(sizeof(ASTList));
//     if(newList==NULL) printError("malloc in appendAST()");
//     newList->data = newChild;
//     newList->next = NULL;
//     parent->childrenTail->next = newList;
//     parent->childrenTail = newList;
//   }
//   return parent;
// }

/* Print the type of this node and any node attributes */
void printNodeTypeAndAttribute(ASTree *t) {
  if(t==NULL) return;
  switch(t->typ) {
    case PROGRAM: printf("PROGRAM"); break;
    case CLASS_DECL_LIST: printf("CLASS_DECL_LIST"); break;
    case CLASS_DECL: printf("CLASS_DECL"); break;
    case VAR_DECL_LIST: printf("VAR_DECL_LIST"); break;
    case VAR_DECL: printf("VAR_DECL"); break;
    case METHOD_DECL_LIST: printf("METHOD_DECL_LIST"); break;
    case METHOD_DECL: printf("METHOD_DECL"); break;
    case NAT_TYPE: printf("NAT_TYPE"); break;
    case OBJ_TYPE: printf("OBJ_TYPE"); break;
    case AST_ID: printf("AST_ID(%s)", t->idVal); break;
    case EXPR_LIST: printf("EXPR_LIST"); break;
    case DOT_METHOD_CALL_EXPR: printf("DOT_METHOD_CALL_EXPR"); break;
    case METHOD_CALL_EXPR: printf("METHOD_CALL_EXPR"); break;
    case DOT_ID_EXPR: printf("DOT_ID_EXPR"); break;
    case ID_EXPR: printf("ID_EXPR"); break;
    case DOT_ASSIGN_EXPR: printf("DOT_ASSIGN_EXPR"); break;
    case ASSIGN_EXPR: printf("ASSIGN_EXPR"); break;
    case PLUS_EXPR: printf("PLUS_EXPR"); break;
    case MINUS_EXPR: printf("MINUS_EXPR"); break;
    case TIMES_EXPR: printf("TIMES_EXPR"); break;
    case EQUALITY_EXPR: printf("EQUALITY_EXPR"); break;
    case GREATER_THAN_EXPR: printf("GREATER_THAN_EXPR"); break;
    case NOT_EXPR: printf("NOT_EXPR"); break;
    case OR_EXPR: printf("OR_EXPR"); break;
    case IF_THEN_ELSE_EXPR: printf("IF_THEN_ELSE_EXPR"); break;
    case FOR_EXPR: printf("FOR_EXPR"); break;
    case PRINT_EXPR: printf("PRINT_EXPR"); break;
    case READ_EXPR: printf("READ_EXPR"); break;
    case THIS_EXPR: printf("THIS_EXPR"); break;
    case NEW_EXPR: printf("NEW_EXPR"); break;
    case NULL_EXPR: printf("NULL_EXPR"); break;
    case NAT_LITERAL_EXPR: printf("NAT_LITERAL_EXPR(%d)",t->natVal); break;
    default: printError("unknown node type in printNodeTypeAndAttribute()");
  }  
}

/* print tree in preorder */
void printASTree(ASTree *t, int depth) {
  if(t==NULL) return;
  printf("%d:",depth);
  int i=0;
  for(; i<depth; i++) printf("  ");
  printNodeTypeAndAttribute(t);
  printf("   (ends on line %d)", t->lineNumber);
  printf("\n");
  //recursively print all children
  ASTList *childListIterator = t->children; //childrenHead
  while(childListIterator!=NULL) {
    printASTree(childListIterator->data, depth+1);
    childListIterator = childListIterator->next;
  }
}

/* Print the AST to stdout with indentations marking tree depth. */
void printAST(ASTree *t) { printASTree(t, 0); }

