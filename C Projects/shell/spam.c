#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define SLEEP_TIME_DEFAULT 5
#define SPAM_AMOUNT_DEFAULT 5

int sleep_time;
int spam_amount;

int main(int argc, char * args[]){
	/* checks */
	if(argc < 2){
		printf("Usage: %s <spam text> [spam intervall in s] [spam amount]\n",args[0]);
		exit(1);
	}
	sleep_time = SLEEP_TIME_DEFAULT;
	spam_amount = SPAM_AMOUNT_DEFAULT;
	if(argc >= 3){
		sleep_time = atoi(args[2]);
	}
	if(argc >= 4){
		spam_amount = atoi(args[3]);
	}
	for(int i = 0; i < spam_amount; i++){
		sleep(sleep_time);
		printf("%s\n",args[1]);
	}
}
