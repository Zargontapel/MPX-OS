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