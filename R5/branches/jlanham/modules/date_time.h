#include <system.h>
#include <string.h>
#include "mpx_supt.h"
#include "polling.h"
#include <core/io.h>
#include <core/serial.h>

void writetoRTC(unsigned char, char *);
void fromBCD(unsigned char, char *);
unsigned char toBCD(char *);
char* toweekday(char *);
char* tomonth(char *);
int isvalid(int, int, int);
int isvalidyear(int);
int isleapyear(int);
int isvalidmonth(int);
int isvalidhour(int);
int isvalidminute(int);
int isvalidseconds(int);
void setyear();
void setmonth();
void setday();
void sethour();
void setminute();
void setseconds();
char* getweekday();
char* getmonth();
char* getmonthday();
char* getyear();
char* gethour();
char* getminute();
char* getsecond();
char* getdatetimeunit(unsigned char);