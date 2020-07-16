#include "SJN.h"
#include "../include/task.h"
#include "../include/Queue.h"
#include <stdlib.h>
#include <stdio.h>

Queue *q;

int comparator_SJN(const void *taskOld, const void *taskNew){
	int lenOld = ((def_task *)taskOld)->length;
	int lenNew = ((def_task *)taskNew)->length;
	int ergebnis;
	if (lenOld >= lenNew){
		ergebnis = 0;
	}
	else {
		ergebnis = 1;
	}
	return ergebnis;
}

int init_SJN()
{
    q = queue_new(*comparator_SJN);
	if ( q == NULL ){
		return 1;
	}
	return 0;
}

void free_SJN()
{
    queue_free(q);
}

void arrive_SJN(int id, int length)
{
    def_task *arriveTask = (def_task*) malloc(sizeof(def_task));
	arriveTask->id = id;
	arriveTask->length = length;
	queue_offer(q, arriveTask);
}

def_task *tick_SJN()
{
	if (running_task == NULL){
		return NULL;
	}
	running_task->length--;
	//Wenn Task anschließend fertsch -> switchen
	if (running_task->length == 0){
		def_task *queueNext = queue_poll(q);
		switch_task(queueNext);
		return running_task;
	}
	return running_task;
}

void finish_task_SJN()
{
    // TODO optional
}
