#include <system.h>
#include <string.h>
#include "polling.h"
#include "date_time.h"

/*
  Procedure..: comhandle
  Description..: Handles command input from the user
*/
void comhandle();

/*
  Procedure..: shutdownConfirmed
  Description..: Confirms whether or not the user wishes to shut down the OS
  Returns..: 1 if the user wishes to shutdown, 0 otherwise
*/
int shutdownConfirmed();

/*
  Procedure..: version
  Description..: Returns the OS version
*/
void version();

/*
  Procedure..: help
  Description..: Prompts the user for a command and gives them helpful information on it
*/
void help();

/*
  Procedure..: setdate
  Description..: Sets the date of the system
*/
void setdate();

/*
  Procedure..: getdate
  Description..: Prints the system date to the screen
*/
void getdate();

/*
  Procedure..: settime
  Description..: Sets the time of the system
*/
void settime();

/*
  Procedure..: gettime
  Description..: Prints the system time to the screen
*/
void gettime();