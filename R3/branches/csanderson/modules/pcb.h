#include <core/serial.h>
#include "mpx_supt.h"
#include "context.h"

typedef struct pcb {
  char* name;
  //Boolean for application - 0 / system - 1 class
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
  u32int* stackBase;
  u32int* stackTop;

  //Linked-List variables
  struct pcb* next;
  struct pcb* previous;
} PCB;

//Allocates memory for a new PCB
//Returns a pointer to the new PCB
PCB* AllocatePCB(int stackSize);

//Frees all memory associated with a given PCB
//Returns either success or an error code
int FreePCB(PCB* process);

void PrintPCB(PCB* process);