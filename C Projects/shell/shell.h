#include <sys/wait.h>
#include <sys/types.h>

#define BUFFER_SIZE 1024
#define PIDS_SIZE 5

/* size of buffer to store user input */
const int input_size = BUFFER_SIZE;
/* buffer to store user input */
char input[BUFFER_SIZE];

/* amount of child processes */
const int children_amount = PIDS_SIZE;
/* array to hold information (pids) about child processes */
pid_t children[PIDS_SIZE];

/* function declarations */
void print_prompt();
void clear_buffer();
void execute_cmd(char **, int);
void shell_exit();
int update_children();
char ** parse_cmd(int *);
