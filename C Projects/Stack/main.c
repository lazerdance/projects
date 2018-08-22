#include "stack.h"
#include <stdio.h>
#include <stdlib.h>
/*
This main function is an example to test the implemented data structure and its functions.
Be advised that further tests will be conducted to grade the submission.
*/

int main()
{

	Stack *stack = stack_new();
    
	char *tempName = NULL;
   
	stack_push(stack, s_elem_new("Null"));
     
	stack_push(stack, s_elem_new("Eins"));

	stack_peek(stack);
     
	stack_push(stack,s_elem_new("Zwei"));

    
	stack_pop(stack, &tempName);

    
	stack_peek(stack);
 
	stack_push(stack, s_elem_new("Zwei"));
	stack_push(stack, s_elem_new("Drei"));
	stack_push(stack, s_elem_new("Vier"));
	stack_peek(stack);
	stack_push(stack, s_elem_new("Fünf"));
	stack_print(stack);
    
/* should print:
  (0,Null)
  (1,Eins)
  (2,Zwei)
  (3,Drei)
  (4,Vier)
  (5,Fünf)
*/
	free(tempName);
	stack_free(stack);
	// use: valgrind ./stack
	// to check if your free function works correctly

	return 0;
}
