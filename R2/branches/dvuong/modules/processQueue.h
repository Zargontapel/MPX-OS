#include "pcb.h"
#include <string.h>

struct queue {
	int count;
	PCB* head;
	PCB* tail;
};

//adds a PCB to the ready queue
//****only used internally****//
void addReady(PCB *);

//adds a PCB to the blocked queue
//****only used internally****//
void addBlocked(PCB *);

//returns and doesn't remove the head of the ready queue
PCB* peekReady();

//returns and doesn't remove the head of the blocked queue
PCB* peekBlocked();

//returns and removes the head of the ready queue
PCB* pollReady();

//returns and removes the head of the blocked queue
PCB* pollBlocked();

PCB* poll(struct queue *);

int lengthReady();

int lengthBlocked();

int indexByNameReady(char *);

int indexByNameBlocked(char *);

int indexByName(struct queue, char *);

PCB* getPCBAtIndexReady(int);

PCB* getPCBAtIndexBlocked(int);

PCB* getPCBAtIndex(struct queue, int);

PCB* removePCBAtIndexReady(int);

PCB* removePCBAtIndexBlocked(int);

PCB* removePCBAtIndex(struct queue *, int);