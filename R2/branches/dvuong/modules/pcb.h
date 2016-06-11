#include "mpx_supt.h"

typedef struct pcb {
  char* name;
  //Boolean for application / system class
  unsigned char class;
  //Priority 0-9
  char priority;
  //Ready, Running, or Blocked
  //   0 ,       1,          2
  unsigned char readyState;
  //Suspended / not suspended
  //       1  /             0
  unsigned char suspendedState;

  //Stack variables
  unsigned char* stackTop;
  unsigned char* stackBase;

  //Linked-List variables
  struct pcb* next;
  struct pcb* previous;
} PCB;

//Allocates memory for a new PCB
//Returns a pointer to the new PCB
PCB* AllocatePCB();

//Frees all memory associated with a given PCB
//Returns either success or an error code
int FreePCB(PCB* process);

//Initializes the PCB information and sets the PCB state to ready, not suspended
//Returns a pointer to the new PCB
PCB* SetupPCB(char* name, unsigned char class, unsigned char priority);

void PrintPCB(PCB* process);
