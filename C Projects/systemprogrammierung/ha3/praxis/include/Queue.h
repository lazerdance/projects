#ifndef QUEUE_H_
#define QUEUE_H_

#include "task.h"

typedef struct Queue Queue;

typedef struct q_elem_s
{
    def_task *task;
    struct q_elem_s *next;
} q_elem;

struct Queue
{
    /*
     * param:
     *      a: def_task struct from task already in queue
     *      b: def_task struct from new arriving task
     *
     * this method should return:
     * 0    if a has higher priority than b
     * > 0  if b has higher priority than a
     *
     * See queue_offer() in Queue.c for further explanation.
     */
    int (*comparator)(const void *a, const void *b);

    int size;

    q_elem *head;
};

/*
 * Creates new Scheduling Queue.
 * The comparator function decides where new arriving tasks should be inserted.
 * See struct Queue for more information on comparator function.
*/
Queue* queue_new(int (*comparator)(const void *a, const void *b));

// Frees a queue and all of its data structures
void queue_free(Queue *queue);

// Pushes task to the front of the scheduling queue
void* queue_push_to_front(Queue *queue, def_task *task);

// Offer new task to the scheduling queue
void* queue_offer(Queue *queue, def_task* value);

// Get first task in the scheduling queue without deleting it from the queue
def_task* queue_peek(Queue *queue);

// Get first task in the scheduling queue and remove it from the queue
def_task* queue_poll(Queue *queue);

// Get amount of tasks in the queue
int queue_size(Queue *queue);

#endif /* QUEUE_H_ */
