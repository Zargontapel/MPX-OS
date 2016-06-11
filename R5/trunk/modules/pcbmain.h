#include "processQueue.h"

//Finds a PCB with a given name
//Returns the given PCB or NULL if not found
PCB* findPCB(char * name); 

//Inserts a PCB into the appropriate queue
void insertPCB(PCB* process);

//Removes a PCB from the queue it is currently in
//Returns either success or an error code
int removePCB(PCB* process);

//Initializes the PCB information and sets the PCB state to ready, not suspended
//Returns a pointer to the new PCB
PCB* SetupPCB(char[], unsigned char, unsigned char, unsigned char, unsigned char, int);