#include <system.h>
#include <string.h>
#include "mpx_supt.h"
#include <core/io.h>

void writetoRTC(unsigned char, char *);
void fromBCD(unsigned char, char *);
unsigned char toBCD(char *);
char* fromweekday(char *);
char* toweekday(char *);
char* frommonth(char *);
char* tomonth(char *);