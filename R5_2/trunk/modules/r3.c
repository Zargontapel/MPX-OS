#include "r3.h"

PCB* cop = NULL;
context *copContext;
extern param params;

u32int* sys_call(context *registers) {
  if (cop == NULL) {
    copContext = registers;
  }
  else {
    if (params.op_code == IDLE) {
      cop->stackTop = (unsigned char*)registers;
      cop->readyState = 0;
      addReady(cop);
      cop = NULL;
    }
    else if (params.op_code == EXIT) {
      FreePCB(cop);
      cop = NULL;
    }
  }
  if (peekReady() != NULL) {
    cop = pollReady();
    cop->readyState = 1;
    return (u32int*)(cop->stackTop);
  } else {
    return (u32int*)copContext;
  }
}

