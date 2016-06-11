#include "pcbmain.h"
#include "mpx_supt.h"

/*
  Procedure..: sys_call
  Description..: Performs a context switch
  Params..: The context of the new process
  Returns the context of the new currently operating process
*/
u32int* sys_call(context *registers);
