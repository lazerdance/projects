#include "../lib/queue.h"
#include <stdlib.h>
#include <stdio.h>
#include "../lib/process.h"

//TODO: Fehlerbehandlung
int queue_add(void* new_object, queue_object* queue){
    queue_object* new_queue_object= calloc(1,sizeof(queue_object));
    if (new_queue_object==NULL){
        return 1;
    }
    new_queue_object->object=new_object;
    new_queue_object->next=queue->next;
    queue->next=new_queue_object;
    return 0;
}

void* queue_poll(queue_object* queue){
    if(queue==NULL || queue->next==NULL){
        return NULL;
    }
    queue_object* object_to_find=queue;
    queue_object* previous_object=queue;
    while(object_to_find->next!=NULL){
        previous_object=object_to_find;
        object_to_find=object_to_find->next;
    }
    previous_object->next=NULL;
    void* object=object_to_find->object;
    free(object_to_find);
    return object;
}

void* queue_poll_hrrn(queue_object* queue, int time){
    if(queue==NULL || queue->next==NULL){
        return NULL;
    }
    queue_object* object_to_find=queue;
    queue_object* object_hrrn=queue->next;
    queue_object* previous_object=queue;
    while(object_to_find->next!=NULL){
        process* p_new = object_to_find->next->object;
        process* p_current = object_hrrn->object;
        
        int wait_time_new = p_new->start_time > time ? 0 : time-p_new->start_time;
        int wait_time_current = p_current->start_time > time ? 0 : time-p_current->start_time;
        float rr_new = (float) (wait_time_new + p_new->time_left) / (float) p_new->time_left;
        float rr_current = (float) (wait_time_current + p_current->time_left) / (float) p_current->time_left;
        if(rr_new > rr_current
            || (rr_new == rr_current && p_new->time_left < p_current->time_left)){
            object_hrrn = object_to_find->next;
            previous_object=object_to_find;
        }
        object_to_find=object_to_find->next;
    }
    previous_object->next=object_hrrn->next;
    void* object=object_hrrn->object;
    free(object_hrrn);
    return object;
}

void* queue_poll_priop(queue_object* queue){
    if(queue==NULL || queue->next==NULL){
        return NULL;
    }
    queue_object* object_to_find=queue;
    queue_object* object_prio=queue->next;
    queue_object* previous_object=queue;
    while(object_to_find->next!=NULL){
        process* p_new = object_to_find->next->object;
        process* p_current = object_prio->object;
        if(p_new->priority > p_current->priority
            || (p_new->priority == p_current->priority && p_new->start_time < p_current->start_time)){
            object_prio = object_to_find->next;
            previous_object=object_to_find;
        }
        object_to_find=object_to_find->next;
    }
    previous_object->next=object_prio->next;
    void* object=object_prio->object;
    free(object_prio);
    return object;
}

void* queue_poll_srtnp(queue_object* queue){
    if(queue==NULL || queue->next==NULL){
        return NULL;
    }
    queue_object* object_to_find=queue;
    queue_object* object_srtn=queue->next;
    queue_object* previous_object=queue;
    while(object_to_find->next!=NULL){
        process* p_new = object_to_find->next->object;
        process* p_current = object_srtn->object;
        if(p_new->time_left < p_current->time_left
            || (p_new->time_left == p_current->time_left && p_new->start_time < p_current->start_time)){
            object_srtn = object_to_find->next;
            previous_object=object_to_find;
        }
        object_to_find=object_to_find->next;
    }
    previous_object->next=object_srtn->next;
    void* object=object_srtn->object;
    free(object_srtn);
    return object;
}

void* queue_pop(queue_object* queue){
    if(queue==NULL || queue->next==NULL){
        return NULL;
    }
    queue_object* object_to_find=queue->next;
    if(object_to_find->next != NULL){
        queue->next = object_to_find->next;
    }else{
        queue->next = NULL;
    }
    void* object=object_to_find->object;
    free(object_to_find);
    return object;
}


queue_object* new_queue(){
    queue_object* new_queue=malloc(sizeof(queue_object));
    if (new_queue==NULL){ //In case of an error
        return 0;
    }
    new_queue->next=NULL;
    new_queue->object=NULL;
    return new_queue;
}


void free_queue(queue_object* queue){
    queue_object* obj_to_delete=queue->next;
    while(obj_to_delete!=NULL){
        queue->next=obj_to_delete->next;
        free(obj_to_delete);
        obj_to_delete=queue->next;
    }
    free(queue);
}

void* queue_peek(queue_object* queue){
    if(queue==NULL || queue->next==NULL){
        return NULL;
    }
    queue_object* object_to_find=queue;
    while(object_to_find->next!=NULL){
        object_to_find=object_to_find->next;
    }
    return object_to_find->object;
}