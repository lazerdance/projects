#include "RR.h"
#include "../include/task.h"
#include "Queue.h"
#include <stdlib.h>

Queue *q;
int RR_time_step;
int RR_current_time_step;

// Alles soll ganz hinten ankommen
int comparator_RR(const void *taskOld, const void *taskNew){
	return 0;
}

int init_RR(int time_step)
{   
    RR_time_step = time_step;
    RR_current_time_step = time_step;
    q = queue_new(*comparator_RR);
	if ( q == NULL ){
		return 1;
	}
	return 0;
}

void free_RR()
{
    queue_free(q);
}

void arrive_RR(int id, int length)
{
    def_task *arriveTask = (def_task*) malloc(sizeof(def_task));
	arriveTask->id = id;
	arriveTask->length = length;
	queue_offer(q, arriveTask);
}

def_task *tick_RR()
{
    if (running_task == NULL){
		return NULL;
	}
	running_task->length--;
	RR_current_time_step--;
	if (running_task->length == 0){
		RR_current_time_step = RR_time_step;
		return switch_task(queue_poll(q));
	}
	if (RR_current_time_step == 0){
		def_task *queueNext = queue_poll(q);
		//### Malloc für safeRunning?
		def_task *safeRunning = switch_task(queueNext);
		queue_offer(q,safeRunning);
		return safeRunning;
	}
	return running_task;
}

void finish_task_RR()
{
    // TODO optional
}
