//PCB.c
//PCB struct
#include "pcb.h"

PCB* AllocatePCB(int stackSize) {
  PCB *newpcb = sys_alloc_mem(sizeof(PCB));
  newpcb->stackBase = sys_alloc_mem(stackSize);
  newpcb->stackTop = newpcb->stackBase + stackSize - sizeof(context);
  return newpcb;
}

int FreePCB(PCB* process) {
  int nameCode, classCode, priorityCode, readyCode;
  int suspCode, sTCode, sBCode;
  int retCode;

  nameCode = sys_free_mem(process->name);
  classCode = sys_free_mem(&(process->class));
  priorityCode = sys_free_mem(&(process->priority));
  readyCode = sys_free_mem(&(process->readyState));
  suspCode = sys_free_mem(&(process->suspendedState));
  sTCode = sys_free_mem(process->stackTop);
  sBCode = sys_free_mem(process->stackBase);

  if (nameCode == -1 || classCode == -1 || priorityCode == -1 ||
      readyCode == -1 || suspCode == -1 || sTCode == -1 ||
      sBCode == -1) {
    retCode = -1;
  }
  else
    retCode = 0;
  return retCode;
}

void PrintPCB(PCB* process) {
  serial_println("");
  serial_println(process->name);
  
  serial_print("Class: ");
  if (process->class == 0) {
    serial_println("Application");
  }
  else if (process->class == 1) {
    serial_println("System");
  }

  serial_print("State: ");
  if (process->readyState == 0) {
    serial_println("Ready");
  }
  else if (process->readyState == 1) {
    serial_println("Running");
  }
  else if (process->readyState == 2) {
    serial_println("Blocked");
  }

  if (process->suspendedState == 0) {
    serial_println("Not Suspended");
  }
  else if (process->suspendedState == 1) {
    serial_println("Suspended");
  }

  serial_print("Priority: ");
  char* priority = sys_alloc_mem(2);
  priority[0] = (char) process->priority + 48;
  priority[1] = '\0';
  serial_println(priority);
  serial_println("");
  sys_free_mem(priority);
}

