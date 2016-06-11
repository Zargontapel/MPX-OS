#include <system.h>
#include <string.h>
#include "mpx_supt.h"
#include "polling.h"
#include <core/io.h>
#include <core/serial.h>

/*
  Procedure..: writetoRTC
  Description..: Writes the encoded time to the Real Time Clock
  Params..: The port to write to, and the time message to encode
*/
void writetoRTC(unsigned char, char *);

/*
  Procedure..: fromBCD
  Description..: Decodes a value from BCD format
  Params..: The bcd value to decode, the decoded value
*/
void fromBCD(unsigned char, char *);

/*
  Procedure..: toBCD
  Description..: Encodes a value to BCD format
  Params..: The value to encode
  Returns..: The encoded BCD value
*/
unsigned char toBCD(char *);

/*
  Procedure..: toweekday
  Description..: Converts a weekday number to the character representation
  Params..: The number of the day of the week
  Returns..: The alphabetical week representation
*/
char* toweekday(char *);

/*
  Procedure..: toweekday
  Description..: Converts a month number to the character representation
  Params..: The number of the day of the month
  Returns..: The alphabetical month representation
*/
char* tomonth(char *);

/*
  Procedure..: isvalid
  Description..: Checks to see if a value is within a set of bounds
  Params..: The value to check, the lowerbound, the higherbound
  Returns..: Returns 1 if the value is valid, 0 otherwise
*/
int isvalid(int, int, int);

/*
  Procedure..: isvalidyear
  Description..: Checks to see if a given year is within 2000-2099
  Params..: The year to check
  Returns..: Returns 1 if the year is valid, 0 otherwise
*/
int isvalidyear(int);

/*
  Procedure..: isleapyear
  Description..: Checks to see if the given year is a leap year
  Params..: The year to check
  Returns..: Returns 1 if the year is a leap year, 0 otherwise
*/
int isleapyear(int);

/*
  Procedure..: isvalidmonth
  Description..: Checks that the given month is between 1 and 12
  Params..: The month to check
  Returns..: Returns 1 if the month is valid, 0 otherwise
*/
int isvalidmonth(int);

/*
  Procedure..: isvalidday
  Description..: Checks that the given day is valid for the month
  Params..: The day to check
  Returns..: Returns 1 if the day is valid, 0 otherwise
*/
int isvalidday(int);

/*
  Procedure..: isvalidhour
  Description..: Checks that the given hour is between 0 and 23
  Params..: The hour to check
  Returns..: Returns 1 if the hour is valid, 0 otherwise
*/
int isvalidhour(int);

/*
  Procedure..: isvalidminute
  Description..: Checks that the given minute is between 0 and 59
  Params..: The minute to check
  Returns..: Returns 1 if the minute value is valid, 0 otherwise
*/

int isvalidminute(int);

/*
  Procedure..: isvalidseconds
  Description..: Checks that the given second is between 0 and 59
  Params..: The month to check
  Returns..: Returns 1 if the month is valid, 0 otherwise
*/

int isvalidseconds(int);

/*
  Procedure..: setyear
  Description..: Prompts the user for the year, checks it for validity, and writes it to the RTC
*/
void setyear();

/*
  Procedure..: setmonth
  Description..: Prompts the user for the month, checks it for validity, and writes it to the RTC
*/
void setmonth();

/*
  Procedure..: setday
  Description..: Prompts the user for the day, checks it for validity, and writes it to the RTC
*/
void setday();

/*
  Procedure..: sethour
  Description..: Prompts the user for the hour, checks it for validity, and writes it to the RTC
*/
void sethour();

/*
  Procedure..: setminute
  Description..: Prompts the user for the minute, checks it for validity, and writes it to the RTC
*/
void setminute();

/*
  Procedure..: setseconds
  Description..: Prompts the user for the seconds, checks it for validity, and writes it to the RTC
*/
void setseconds();

/*
  Procedure..: getweekday
  Description..: Gets the weekday from the system
  Returns..: The system weekday
*/
char* getweekday();

/*
  Procedure..: getmonth
  Description..: Gets the month from the system
  Returns..: The system month
*/
char* getmonth();

/*
  Procedure..: getmonthday
  Description..: Gets the day of the month from the system
  Returns..: The system day of the month
*/
char* getmonthday();

/*
  Procedure..: getyear
  Description..: Gets the year from the system
  Returns..: The system year
*/
char* getyear();

/*
  Procedure..: gethour
  Description..: Gets the hour from the system
  Returns..: The system hour
*/
char* gethour();

/*
  Procedure..: getminute
  Description..: Gets the minute from the system
  Returns..: The system minute
*/
char* getminute();

/*
  Procedure..: getsecond
  Description..: Gets the second from the system
  Returns..: The system second
*/
char* getsecond();

/*
  Procedure..: getdatetimeunit
  Description..: Gets the date time value from the system port
  Params..: The port to get the value from
  Returns..: The system value
*/
char* getdatetimeunit(unsigned char);