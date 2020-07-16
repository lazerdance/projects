#include "queue.h"

node* create_node(void* data, unsigned int size)
{
	node* new = (node*)malloc(sizeof(node));
	new->size = size;
	new->data = data;

	new->next = NULL;
	new->prev = NULL;
	return new;
}

void free_node(node* n)
{
	free(n);
}

void free_Q(queue* q)
{
	if(q==NULL) return;
	node* current = q->head;
	node* next;

	while(current!=NULL)
	{
		next = current->prev;
		free_node(current);
		current = next;
	}
	q->head = NULL;
	q->tail = NULL;
}

queue* init_Q(queue* q)
{
	if(q == NULL)
	{
		printf("ERROR: init_Q()\n");
		return NULL;
	}
	q->size=0;
	q->head=NULL;
	q->tail=NULL;
	return q;
}

void append_node(node* n, queue* q)
{
	if(n == NULL || q == NULL)
	{
		printf("ERROR: append_node()\n");
		return;
	}
	if(q->head==NULL)
	{
		q->head = n;
		q->tail = n;
	}else
	{
		n->next = q->tail;
		n->prev = NULL;
		q->tail->prev = n;
		q->tail = n;
	}
	q->size++;
}

node* remove_node(node* to_remove, queue* q)
{
	if(to_remove == NULL || q == NULL)
	{
		printf("ERROR: remove_node()\n");
		return NULL;
	}
	node* tmp = q->head;

	while(tmp!=NULL)
	{

		if(tmp == to_remove)
		{
			if(tmp == q->head) q->head = tmp->prev;
			if(tmp == q->tail) q->tail = tmp->next;
			if(tmp->next!=NULL)tmp->next->prev = tmp->prev;
			if(tmp->prev!=NULL)tmp->prev->next = tmp->next;
			q->size--;
			return tmp;
		}
		tmp = tmp->prev;
	}

	return NULL;
}