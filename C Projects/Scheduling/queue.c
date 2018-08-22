#include "../Tasks/TaskPool.h"
#include <stdio.h>
#include "queue.h"
#include <stdlib.h>



queue* create_queue(){
    queue* new_queue = (queue*) malloc(sizeof(queue));
    new_queue->head = NULL;
    new_queue->last = NULL;
    return new_queue;
}

void add_elem(queue* q, Task* t){
    queueElem* new_elem = (queueElem*) malloc(sizeof(queueElem));
    new_elem->task = t;
    new_elem->next = NULL;
    if(q->head == NULL){
        q->head = new_elem;
        q->last = new_elem;
    }else{
        q->last->next = new_elem;
        q->last = new_elem;
    }
    return;
}

Task* poll(queue* q){
    if(q->head == NULL){
        return NULL;
    }
    queueElem* remove = q->head;
    Task* tmp = q->head->task;
    q->head = q->head->next;
    free(remove);
    return tmp;
}

int isEmpty(queue* q){
    if(q->head == NULL)
        return 1;
    else
        return 0;
}

void delete_queue(queue* q){
    free(q);
    return;
}



