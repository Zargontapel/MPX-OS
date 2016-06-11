#include <string.h>
#include <core/serial.h>
#include "mpx_supt.h"
#include <core/io.h>

#define BUFFERSIZE 25
#define MAXVALUESIZE 2
#define NULL_CHAR_VALUE 8

/*
  Procedure..: polling
  Description..: Reads in user input and adds it to the buffer
  Returns..: The read in buffer
*/
char* polling();
