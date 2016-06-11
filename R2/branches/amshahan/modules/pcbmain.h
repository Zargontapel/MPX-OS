#include "processQueue.h"

//Finds a PCB with a given name
//Returns the given PCB or NULL if not found
PCB* findPCB(char * name); 

//Inserts a PCB into the appropriate queue
void insertPCB(PCB* process);

//Removes a PCB from the queue it is currently in
//Returns either success or an error code
int removePCB(PCB* process);