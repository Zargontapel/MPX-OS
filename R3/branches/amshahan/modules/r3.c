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
      cop->stackTop = (u32int*)registers;
      addReady(cop);
    }
    else if (params.op_code == EXIT) {
      FreePCB(cop);
    }
    if (peekReady() == NULL) {
      cop = pollReady();
      cop->readyState = 1;
    }
  }
  return cop->stackTop;
}

