#include "SRTN.h"
#include "../include/task.h"
#include "../include/Queue.h"
#include <stdlib.h>
#include <stdio.h>


Queue *q;

int comparator_SRTN(const void *taskOld, const void *taskNew){
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

int init_SRTN()
{
    q = queue_new(*comparator_SRTN);
	if ( q == NULL ){
		return 1;
	}
	return 0;
}

void free_SRTN()
{
    queue_free(q);
}

void arrive_SRTN(int id, int length)
{
	//### Malloc fehlt
    def_task *arriveTask = (def_task*) malloc(sizeof(def_task));
	arriveTask->id = id;
	arriveTask->length = length;
	queue_offer(q,arriveTask);		
}

def_task *tick_SRTN()
{
	if (running_task == NULL){
		return NULL;
	}
	running_task->length--;
	if (running_task->length == 0){
		return switch_task(queue_poll(q));
	}
	if (running_task->length > queue_peek(q)->length){
		def_task *queueNext = queue_poll(q);
		//### Malloc für safeRunning?
		def_task *safeRunning = switch_task(queueNext);
		queue_offer(q,safeRunning);
		return safeRunning;
	}
	return running_task;
}

void finish_task_SRTN()
{
    // TODO optional
}