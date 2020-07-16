#include "LCFS-PR.h"
#include "../include/task.h"
#include "../include/Queue.h"
#include <stdlib.h>
#include <stdio.h>


Queue *q;

int comparator_LCFS(const void *taskOld, const void *taskNew){
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
	
int init_LCFS_PR()
{
    q = queue_new(*comparator_LCFS);
	if ( q == NULL ){
		return 1;
	}
	return 0;
}

void free_LCFS_PR()
{
    queue_free(q);
}

void arrive_LCFS_PR(int id, int length)
{
    def_task *arriveTask = (def_task*) malloc(sizeof(def_task));
	arriveTask->id = id;
	arriveTask->length = length;
	queue_push_to_front(q,arriveTask);
}

def_task *tick_LCFS_PR()
{
	if (running_task == NULL){
		return NULL;
	}
	running_task->length--;
	if (running_task->length == 0){
		def_task *queueNext = queue_poll(q);
		switch_task(queueNext);
		return running_task;
	}
    return running_task;
}

void finish_task_LCFS_PR()
{
    // todo optional
}