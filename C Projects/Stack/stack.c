#include "stack.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Stack
{
	int size;
	s_elem *head;
};
/* Beginning of assignment -------------------------------------------------------------------------- */
struct s_elem
{
    char* name;
    int index;
    s_elem* next;
	// TODO: One element of a stack consists of a name, an integer index and a pointer to the element under this one (predecessor)
};

Stack* stack_new()
{
    
	//TODO
    Stack* stackptr = malloc(sizeof(Stack));

	//Chech if memory could be allocated

	if(stackptr == NULL){
		perror("Probleme beim Speicher allozieren in stack_new()\n");
		return NULL;
	}
	//Initialising Stack
    stackptr->head=NULL;
    stackptr->size=0;
	return stackptr;
}

s_elem* s_elem_new(char* name)
{
	//TODO
    s_elem* new_elem= malloc(sizeof(s_elem));
    new_elem->name = malloc((strlen(name)+1)*sizeof(char));

	if(new_elem == NULL){
		
		perror("Probleme beim Speicher allozieren in elem_new()\n");
		return NULL;
	}
	if(new_elem->name == NULL) {
		perror("Probleme beim Speicher allozieren in elem_new()\n");
		return NULL;
	}

    strcpy(new_elem->name, name);
    new_elem->next=NULL;
    new_elem->index = 0;
	return new_elem;
}

void stack_free(Stack *stack)
{
	
      
    while(stack->head != NULL){
        s_elem* helpptr = stack->head;
        stack->head = stack->head->next;
        free(helpptr->name);
        free(helpptr);
    }
    free(stack);
    
}

char* stack_push(Stack *stack, s_elem* newElem)
{
  //TODO
    if(newElem==NULL) return NULL;

    if(stack->head ==NULL){
        stack->head = newElem;
        stack->size = 1;
        stack->head->index =0;
        stack->head->next=NULL;
        return stack->head->name;
    }else{
        newElem->next = stack->head;
        newElem->index = stack->head->index+1;
        stack->head=newElem;
        stack->size+=1;
        return stack->head->name;
    }
}

char* stack_peek(Stack *stack)
{
	//TODO
    if(stack->head == NULL) return NULL;
    return stack->head->name;
}

void stack_pop(Stack *stack, char **name)
{
	//TODO
    if(stack->head == NULL){
        name = NULL;
        return;
    }
   
    int nameLength = strlen(stack->head->name);
    
    (*name) = (char*)malloc((nameLength+1)*sizeof(char));

	if((*name)==NULL){
		perror("Probleme beim Speicher allozieren in elem_new()\n");
		return;
	}

    strcpy(*name, stack->head->name);
    stack->size-=1;
    s_elem* helpptr = stack->head;
    stack->head= stack->head->next;
    free(helpptr->name);
    free(helpptr);
      
}

int stack_size(Stack *stack){
	//TODO
  return stack->size;
}

void stack_print(Stack *stack)
{
 	//TODO
    if(stack->head == NULL) return;
   // printf("(%d, %s)\n", stack->head->index, stack->head->name);
    s_elem* helpptr = stack->head;
    while(helpptr != NULL){
        printf("(%d,%s)\n", helpptr->index, helpptr->name);
        helpptr = helpptr->next;
    } 
}
/* End of assignment -------------------------------------------------------------------------- */
