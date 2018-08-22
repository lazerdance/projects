#ifndef QUEUE_H
#define QUEUE_H
typedef struct _queueElem{
    struct _queueElem* next;
    Task* task;
} queueElem;

typedef struct _queue{
    queueElem* head;
    queueElem* last;

} queue;
queue* create_queue();
void add_elem(queue* q, Task* t);
Task* poll(queue* q);
int isEmpty(queue* q);
void delete_queue(queue*q);
#endif
