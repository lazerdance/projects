typedef struct Stack Stack;
typedef struct s_elem s_elem;
/**
 * Creates an empty stack.
 *
 * returns: an empty stack.
 */
Stack* stack_new();
/**
 * Creates a new stack element.
 * name: the elements name
 *
 * returns the new element
 */
s_elem* s_elem_new(char* name);

/**
 * Deletes the stack and frees all allocated memory.
 */
void stack_free(Stack *stack);

/*
 * Inserts an element to the stack and sets its index.
 *
 * newElem: new element to be inserted
 *
 * returns : the name and index of the element or NULL when an error occurred.
 */
char* stack_push(Stack *stack, s_elem* newElem);

/**
 * Retrieves the top element from the stack.
 *
 * returns: the name or NULL if stack is empty..
 */
char* stack_peek(Stack *stack);

/**
 * Retrieves and REMOVES the top element from the stack.
 *
 * returns: the name or NULL if stack is empty. Memory for the name should be allocated in the function.
 */
void stack_pop(Stack *stack, char **name);

/**
 * The current size of the stack.
 *
 * returns: the current size of the stack (number of elements)
 */
int stack_size(Stack *stack);

/**
 * Prints the current elements on the stack to stdout.
 *
 * The elements get printed in the following form:
 *
 * (<index>,<name>)
 * (<index>,<name>)
 * (<index>,<name>) ...
 *
 * with the head of the stack as first element.
 */
void stack_print(Stack *stack);
