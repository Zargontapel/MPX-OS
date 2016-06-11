#include "r3.h"
#include <core/serial.h>

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
      cop->readyState = 0;
      addReady(cop);
      cop = NULL;
    }
    else if (params.op_code == EXIT) {
      FreePCB(cop);
      cop = NULL;
    }
  }
  PCB* readyUnsuspendedProcess = getNextUnsuspended();
  if (readyUnsuspendedProcess != NULL) {
    cop = readyUnsuspendedProcess;
    cop->readyState = 1;
    //serial_println(cop->name);
    return cop->stackTop;
  } else {
    return (u32int*)copContext;
  }
}
