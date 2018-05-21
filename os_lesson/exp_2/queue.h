#ifndef __QUEUE_H__
#define __QUEUE_H__
#include "common.h"

typedef struct _queue_item
{
	char action[20];
	int data;
	size_t sz;
	
	struct _queue_item *next;
} QUEUE_ITEM;

typedef struct _queue
{
	size_t numitems;
	QUEUE_ITEM *items;
	
	pthread_mutex_t modify_mutex;
	pthread_mutex_t read_mutex;
} QUEUE;

int is_Queue_empty(QUEUE *queue);
QUEUE *Initialize_Queue(void);
void Add_Queue_Item(QUEUE *, char *, int, size_t);
QUEUE_ITEM *Get_Queue_Item(QUEUE *);
void Free_Queue_Item(QUEUE_ITEM *);


#endif