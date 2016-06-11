#include "pcbmain.h"

PCB* findPCB(char* name)
{
  PCB *retPCB;
  int readyIndex = indexByNameReady(name);
  int blockedIndex = indexByNameBlocked(name);
  if (readyIndex >= 0) {
    retPCB = getPCBAtIndexReady(readyIndex);
  } else if (blockedIndex >= 0) {
    retPCB = getPCBAtIndexBlocked(blockedIndex);
  } else {
    retPCB = NULL;
  }
  return retPCB;
}

void insertPCB(PCB* proc)
{
  if(proc->readyState == 0) //Check if ready
  {
    addReady(proc);
  }
  else
    addBlocked(proc);
  
}

int removePCB(PCB* proc)
{
  PCB *delPCB;
  int readyIndex = indexByNameReady(proc->name);
  int blockedIndex = indexByNameBlocked(proc->name);
  if (readyIndex >= 0) {
    delPCB = removePCBAtIndexReady(readyIndex);
  } else if (blockedIndex >= 0) {
    delPCB = removePCBAtIndexBlocked(blockedIndex);
  } else {
    delPCB = NULL;
  }
  return FreePCB(delPCB);
}

PCB* SetupPCB(char name[], unsigned char class, unsigned char priority, unsigned char readyState, unsigned char suspendedState, int stackSize)
{
  PCB* newPCB = NULL;
  if ((indexByNameReady(name) >= 1) || (indexByNameBlocked(name) >= 1)) {
    serial_println("The given process name is currently in use. No PCB created.");
  } else if (strlen(name) == -1) {
    serial_println("No name was given. No PCB created.");
  } else if (class != 0 && class != 1) {
    serial_println("Invalid process class provided. No PCB created.");
  } else if (priority > 9) {
    serial_println("Invalid process priority provided. No PCB created.");
  } else if (readyState != 0 && readyState != 2) {
    serial_println("Invalid process ready state provided. No PCB created.");
  } else if (suspendedState != 0 && suspendedState != 1) {
    serial_println("Invalid process suspended state provided. No PCB created.");
  } else if (stackSize < 0) {
    serial_println("Invalid stack size provided. No PCB created.");
  } else {
    newPCB = AllocatePCB(stackSize);
    newPCB->name = name;
    newPCB->class = class;
    newPCB->priority = priority;
    newPCB->readyState = readyState;
    newPCB->suspendedState = suspendedState;
  }
  return newPCB;
}
