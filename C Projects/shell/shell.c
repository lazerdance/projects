#include "./shell.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(){
	int bg;
	char ** args;

	while(1){
		/* next shell prompt */
		print_prompt();

		/* wait for input */
		args = parse_cmd(&bg);

		/* if valid input, execute */
		if(args){
			execute_cmd(args,bg);
		}
		else {
			printf("Invalid input\n");
		}

		/* free args */
		free(args);
	}	
}

/* prints a beautiful prompt */ 
void print_prompt(){
	printf("\x1B[31mss2018\x1B[0m@\x1B[34msysprog\x1B[0m: ");
}

/* parse commands and allocates memory for it */
char ** parse_cmd(int * bg){

	/* check for EOF */
	if(feof(stdin))exit(1);

	/* check input */
	if(!fgets(input,input_size,stdin))return NULL;
	if(strlen(input) <= 1)return NULL;

	/* Setting up args, argc and bg*/
	int argc = 1;
	char ** args = NULL;
	*bg = 0;

	/* parsing arguments */
	char * next = strtok(input," \n");

	/* check for unparsable input */
	if(!next)return NULL;

	/* check if exiting */
	if(!strcmp(next,"exit")){
		shell_exit();
	}

	/* parse */
	args = (char**) malloc(sizeof(char**));
	args[0] = next;
	while( (next = strtok(NULL," \n")) ){
		args = (char **)realloc(args,sizeof(char**) * (argc + 1));
		args[argc] = next;
		argc++;
	}

	/* check if meant to be executed in the background */
	if(!strcmp(args[argc - 1],"&") && argc > 1){
		*bg = 1;
		args[argc-1] = NULL;
	}
	/* more space to terminate with null pointer */
	else {
		args = (char **)realloc(args,sizeof(char**) * (argc + 1));
		args[argc] = NULL;
	}

	return args;
}

/* --- Platz für eigene Hilfsfunktionen --- */




/* ---------------------------------------- */

/* execute command
 * char * args[]: (arguments)
 *	Enthält den Dateinamen des Auszuführenden Programmes und die
 *	 dazu übergebenen Argumente
 *	args[0]	      --> Dateiname des Programmes
 *	args[1 bis n] --> Argumente
 *	args[n+1]     --> NULL (Nicht wichtig für die Aufgabe)
 *	Gleicher Aufgbau wie char * args[] in der main Methode üblich ist
 *
 * int bg: (background)
 *	flag ob der Prozess im Hintergrund ausgeführt werden soll oder die Shell
 *	 auf den Prozess warten soll
 *	0       --> Shell soll auf den Prozess warten
 *	nicht 0 --> Prozess soll im Hintergrund ausgeführt werden
 */
void execute_cmd(char * args[], int bg){

	/* Relevant für Aufgabe 2 */
	/* clean up children and check if space available */
	if(update_children() >= children_amount){
		printf("Too many processes already running\n");
		return;
	}
    


	/* TODO Your code here -- Aufgabe 1 */
    pid_t PID = fork();
    
    if(PID < 0 ){
        perror("invalid PID in FORK()");
        exit(-1);    
    }
    if(PID==0){
         //Child-process
        //Executes 
        execvp(args[0], args);
		printf("invalid input: don't know ' %s '\n", args[0]);//if execvp didn't work, abort()
		abort();
       
    }else{
        // parent-process
        //add child PID to children[]
        for(int i = 0; i<PIDS_SIZE; ++i){	
            if(children[i]==0){
                 children[i]=PID;
                   break;
            }
            
        }
        if(bg == 0) waitpid(PID,NULL,0); //shell waits for child if bg ==0
        
    }

}



/* Diese Funktion soll alle Child Prozesse die in dem Array "pid_t children[]"
 *  gespeichert sind darauf überprüfen, ob diese noch laufen oder bereits
 *  beendet sind. Wenn ein Prozess beendet wurde soll der Eintrag in dem Array
 *  gelöscht werden, so dass dieser wieder zur Verfügung steht.
 *
 *  return value:
 *   Diese Funktion soll die Anzahl der momentan im Hintergrund laufenden
 *   Prozesse zurückgeben. Prozesse die beendet wurden zählen nicht dazu
 */
int update_children(){

	/* TODO Your code here -- Aufgabe 2 */
    int counter = 0;
    
    for(int i =0; i<PIDS_SIZE;++i){
        if(waitpid(children[i],NULL, WNOHANG)!=0){ //Checks if PID is still active if not active, reset
            children[i] = 0;
        }else{
             if(children[i]!=0) ++counter;
        }
     
       
    }
	return counter;
}

/* Diese Funktion wird aufgerufen, falls das Stichwort "exit" in der Shell
 *  eingetippt wird. Diese Funktion beendet die Shell, jedoch soll sie zuerst darauf
 *  warten, dass alle Hintergrundprozesse beendet wurden.
 */
void shell_exit(){

	/* TODO Your code here -- Aufgabe 3 */
    if(update_children()>0){									//if Child processes still running
    printf("Processes still running %d\n", update_children());
        for(int i = 0; i<PIDS_SIZE;++i){
            if(children[i] != 0){
				printf("Wait for Process with PID: %d\n", children[i]);	
             	waitpid(children[i], NULL, 0);  				//Wait for Child process
              	            
            }        
        }
    }

	printf("Shell exiting\n");
	exit(0);
}


