#include <system.h>
#include <string.h>
#include "polling.h"
#include "date_time.h"
#include "procsr3.h"

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

/*
  Procedure..: suspendPCB
  Description..: Prompts the user for a given process name and suspends it
*/
void suspendPCB();

/*
  Procedure..: resumePCB
  Description..: Prompts the user for a given process name and resumes it
*/
void resumePCB();

/*
  Procedure..: setPriority
  Description..: Prompts the user for a given process and new priority and changes sets the new priority
*/
void setPriority();

/*
  Procedure..: showPCB
  Description..: Prompts the user for a given process and displays its information
*/
void showPCB();

/*
  Procedure..: showReady
  Description..: Displays the information about all processes in the ready queue
*/
void showReady();

/*
  Procedure..: showBlocked
  Description..: Displays the information about all processes in the blocked queue
*/
void showBlocked();

/*
  Procedure..: showAll
  Description..: Displays the information about all processes
*/
void showAll();

/*
  Procedure..: createPCB
  Description..: Prompts the user for information to create a new process
*/
void createPCB();

/*
  Procedure..: deletePCB
  Description..: Prompts the user for a given process and deletes it
*/
void deletePCB();

/*
  Procedure..: block
  Description..: Prompts the user for a given process and blocks it
*/
void block();

/*
  Procedure..: unblock
  Description..: Prompts the user for a given process and unblocks it
*/
void unblock();

/*
  Procedure..: loadR3
  Description..: Loads the processes for R3 into the system
*/
void loadR3();

/*
  Procedure..: loadBasicFunctionToPCB
  Description..: Creates a PCB with the configuration from the parameters and loads it into the system
*/
void loadBasicFunctionToPCB(char[], unsigned char, unsigned char, unsigned char, unsigned char, int, void (*func)());