#include "processQueue.h"

PCB dummyready = {NULL, 1, 0, 0, 0, NULL, NULL, NULL, NULL};
PCB dummyblocked = {NULL, 1, 0, 0, 0, NULL, NULL, NULL, NULL};

struct queue ready = {0, NULL, NULL};

struct queue blocked = {0, NULL, NULL};

void addReady(PCB *proc)
{
	PCB *current = ready.head;
	if (ready.count == 0) { //check if inserting into empty queue
		proc->next = NULL;
		proc->previous = NULL;
		current = proc;
	} else {
		while(current->next != NULL && proc->priority <= current->priority)
		{
			current = current->next;
		}
		if (proc->priority > current->priority) {
			proc->next = current;
			proc->previous = current->previous;
			if (current->previous != NULL) { //Check if inserting at head
				proc->previous->next = proc;
			}
			current->previous = proc;
		} else { //Inserting at tail
			proc->next = NULL;
			proc->previous = current;
			current->next = proc;
		}
	}
	if (proc->previous == NULL)
	{
		ready.head = proc;
	}
	if (proc->next == NULL)
	{
		ready.tail = proc;
	}
	ready.count++;
}

void addBlocked(PCB *proc)
{
	if (blocked.count == 0) { //check if inserting into empty queue
		proc->next = NULL;
		proc->previous = NULL;
		blocked.head = proc;
	} else {
		proc->previous = blocked.tail;
		blocked.tail->next = proc;
	}
	blocked.tail = proc;
	blocked.count++;
}

PCB* peekReady()
{
	return ready.head;
}

PCB* peekBlocked()
{
	return blocked.head;
}

PCB* pollReady()
{
	return poll(&ready);
}

PCB* pollBlocked()
{
	return poll(&blocked);
}

PCB* poll(struct queue *q)
{
	PCB *temp = q->head;
	if (q->head != NULL) {
		q->head = q->head->next;
		q->head->previous = NULL;
		temp->next = NULL;
		q->count--;
	}
	return temp;
}

int lengthReady()
{
	return ready.count;
}

int lengthBlocked()
{
	return blocked.count;
}

int indexByNameReady(char *pcbname)
{
	return indexByName(ready, pcbname);
}

int indexByNameBlocked(char *pcbname)
{
	return indexByName(blocked, pcbname);
}

int indexByName(struct queue q, char *pcbname)
{
	int index = -1;
	int i = 0;
	PCB *current = q.head;
	while (current != NULL) {
		if (!strcmp(pcbname, current->name)) {
			index = i;
			break;
		}
		i++;
		current = current->next;
	}
	return index;
}

PCB* getPCBAtIndexReady(int index)
{
	return getPCBAtIndex(ready, index);
}

PCB* getPCBAtIndexBlocked(int index)
{
	return getPCBAtIndex(blocked, index);
}

PCB* getPCBAtIndex(struct queue q, int index)
{
	PCB *retPCB = q.head;
	if (index < 0 || index >= q.count) {
		retPCB = NULL;
	} else {
		int i = 0;
		for(; i < index; i++) {
			retPCB = retPCB->next;
		}
	}
	return retPCB;
}

PCB* removePCBAtIndexReady(int index)
{
	return removePCBAtIndex(&ready, index);
}

PCB* removePCBAtIndexBlocked(int index)
{
	return removePCBAtIndex(&blocked, index);
}

PCB* removePCBAtIndex(struct queue *q, int index)
{
	PCB *retPCB = q->head;
	if (index < 0 || index >= q->count) {
		retPCB = NULL;
	} else {
		int i = 0;
		for(; i < index; i++) {
			retPCB = retPCB->next;
		}
	}
	if (index == 0) {
		q->head = retPCB->next;
	}
	if (index == q->count - 1) {
		q->tail = retPCB->previous;
	}
	if (retPCB->previous != NULL) {
		retPCB->previous->next = retPCB->next;
	}
	if (retPCB->next != NULL) {
		retPCB->next->previous = retPCB->previous;
	}
	if (retPCB != NULL) {
		q->count--;
	}
	retPCB->previous = NULL;
	retPCB->next = NULL;
	return retPCB;
}

void clearAllQueues()
{
	clearReady();
	clearBlocked();
}

void clearReady()
{
	clear(&ready);
}

void clearBlocked()
{
	clear(&blocked);
}

void clear(struct queue *q)
{
	PCB *current = q->head;
	while (current != NULL) {
		current = current->next;
		PCB *qhead = poll(q);
		FreePCB(qhead);
	}
}