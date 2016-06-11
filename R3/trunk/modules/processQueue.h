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

//Returns and removes the head of a given queue
PCB* poll(struct queue *);

//Returns the length of the ready queue
int lengthReady();

//Returns the length of the blocked queue
int lengthBlocked();

//Returns the index of the given process name from the ready queue 
int indexByNameReady(char *);

//Returns the index of the given process name from the blocked queue
int indexByNameBlocked(char *);

//Returns the index of the given process name from the given queue
int indexByName(struct queue, char *);

//Returns the PCB at the given index of the ready queue
PCB* getPCBAtIndexReady(int);

//Returns the PCB at the given index of the blocked queue
PCB* getPCBAtIndexBlocked(int);

//Returns the PCB at the given index of the given queue
PCB* getPCBAtIndex(struct queue, int);

//Removes and returns the PCB at the given index of the ready queue
PCB* removePCBAtIndexReady(int);

//Removes and returns the PCB at the given index of the blocked queue
PCB* removePCBAtIndexBlocked(int);

//Removes and returns the PCB at the given index of the given queue
PCB* removePCBAtIndex(struct queue *, int);

//Frees all processes in all queue
void clearAllQueues();

//Frees all processes in the ready queue
void clearReady();

//Frees all processes in the blocked queue
void clearBlocked();

//Frees all processes in a given queue
void clear(struct queue *q);