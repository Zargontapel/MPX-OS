#include "com_handler.h"
#include "pcbmain.h"

void comhandle()
{
	serial_println("");
	serial_println("Welcome to the MPX OS.");
	serial_println("Feel free to begin entering commands.");
	serial_println("");
	while(1) {
		char *command = polling();
		if (!strcmpigncase(command, "shutdown")) {
			if (shutdownConfirmed()) {
				serial_println("System shutting down...");
				clearAllQueues();
				break;
			} else {
				serial_println("Shutdown canceled.");
			}
		} else if (!strcmpigncase(command, "version")) {
			version();
		} else if (!strcmpigncase(command, "help")) {
			help();
		} else if (!strcmpigncase(command, "setdate")) {
			setdate();
		} else if (!strcmpigncase(command, "getdate")) {
			getdate();
		} else if (!strcmpigncase(command, "settime")) {
			settime();
		} else if (!strcmpigncase(command, "gettime")) {
			gettime();
		} else if (!strcmpigncase(command, "suspend")) {
			suspendPCB();
		} else if (!strcmpigncase(command, "resume")) {
			resumePCB();
		} else if (!strcmpigncase(command, "setpriority")) {
			setPriority();
		} else if (!strcmpigncase(command, "createPCB")) {
			createPCB();
		} else if (!strcmpigncase(command, "deletePCB")) {
			deletePCB();
		} else if (!strcmpigncase(command, "block")) {
			block();
		} else if (!strcmpigncase(command, "unblock")) {
			unblock();
		} else if (!strcmpigncase(command, "showPCB")) {
		        showPCB();
		} else if (!strcmpigncase(command, "showReady")) {
		        showReady();
		} else if (!strcmpigncase(command, "showBlocked")) {
		        showBlocked();
		} else if (!strcmpigncase(command, "showAll")) {
		        showAll();
		} else if (!strcmpigncase(command, "yield")) {
		        asm volatile ("int $60");
		} else if (!strcmpigncase(command, "loadr3")) {
		        loadR3();
		} else if (strcmp(command, '\0')) {
			serial_println("Command not recognized. Type help to view commands.");
			serial_println("");
		}
		sys_free_mem(command);
	}
}

int shutdownConfirmed()
{
	while(1) {
		serial_println("Are you sure you would like to shutdown? (Y/N)");
		char *response = polling();
		if (!strcmpigncase(response, "y") || !strcmpigncase(response, "yes")) {
			sys_free_mem(response);
			return 1;
		} else if (!strcmpigncase(response, "n") || !strcmpigncase(response, "no")) {
			sys_free_mem(response);
			return 0;
		} else {
			sys_free_mem(response);
			serial_println("Entered value not recognized.");
		}
	}
}

void version()
{
	serial_println("Version 1.0 Module R2");
	serial_println("Completion Date: Thursday, February 26, 2015");
	serial_println("");
}

void help()
{
	while(1) {
		serial_println("Which command would you like to know more about?");
		serial_println("Enter \"commands\" for a list of commands.");
		serial_println("Enter \"quit\" to exit.");
		char *command = polling();
		if (!strcmpigncase(command, "commands")) {
			serial_println("Here's a list of available commands:");
			serial_println("Version");
			serial_println("Help");
			serial_println("Setdate");
			serial_println("Getdate");
			serial_println("Settime");
			serial_println("Gettime");
			serial_println("Shutdown");
			serial_println("Suspend");
			serial_println("Resume");
			serial_println("SetPriority");
			serial_println("ShowPCB");
			serial_println("ShowAll");
			serial_println("ShowReady");
			serial_println("ShowBlocked");
			serial_println("CreatePCB");
			serial_println("DeletePCB");
			serial_println("Block");
			serial_println("Unblock");
			serial_println("");
		}
		else if (!strcmpigncase(command, "quit")) {
			break;
		}
		else if (!strcmpigncase(command, "version")) {
			serial_println("Version shows the current version of the operating system, and it's release date.");
			break;
		}
		else if (!strcmpigncase(command, "help")) {
			serial_println("Help gets further help for a prompted command.");
			break;
		}
		else if (!strcmpigncase(command, "setdate")) {
			serial_println("Setdate allows the user to set the date of the operating system through a series of prompts.");
			serial_println("The prompt follows a format of yy mm dd.");
			break;
		}
		else if (!strcmpigncase(command, "getdate")) {
			serial_println("Getdate returns the current date of the operating system.");
			break;
		}
		else if (!strcmpigncase(command, "settime")) {
			serial_println("Settime allows the user to set the time of the operating system through a series of prompts.");
			serial_println("The prompt follows a format of HH MM SS.");
			break;
		}
		else if (!strcmpigncase(command, "gettime")) {
			serial_println("Gettime returns the current date of the operating system.");
			break;
		}
		else if (!strcmpigncase(command, "shutdown")) {
			serial_println("Shutdown presents the user with the option to terminate the system.");
			break;
		}
		else if (!strcmpigncase(command, "suspend"))  {
		  serial_println("Places a PCB in a suspended state and reinserts it into the appropriate queue.");
		  break;
		}

		else if (!strcmpigncase(command, "resume"))  {
		  serial_println("Places a PCB in in the not suspended state and reinserts it into the appropriate queue.");
		  break;
		}

		else if (!strcmpigncase(command, "setpriority"))  {
		  serial_println("Sets a PCB's priority and reinserts the process into the correct place in the correct queue");
		  break;
		}

		else if (!strcmpigncase(command, "showPCB"))  {
		  serial_println("Displays the following information for a PCB:");
		  serial_println("Process Name");
			serial_println("Class");
			serial_println("State");
			serial_println("Suspended Status");
			serial_println("Priority");
			serial_println("");
		  break;
		}

		else if (!strcmpigncase(command, "showall"))  {
		  serial_println("Displays the folling information for each PCB in the ready and blocked queues.");
		  serial_println("Process Name");
			serial_println("Class");
			serial_println("State");
			serial_println("Suspended Status");
			serial_println("Priority");
			serial_println("");
		  break;
		}

		else if (!strcmpigncase(command, "showready"))  {
		  serial_println("Displays the folling information for each PCB in the ready queue.");
		  serial_println("Process Name");
			serial_println("Class");
			serial_println("State");
			serial_println("Suspended Status");
			serial_println("Priority");
			serial_println("");
		  break;
		}

		else if (!strcmpigncase(command, "showblocked"))  {
		  serial_println("Displays the following information for each PCB in the blocked queue.");
		  serial_println("Process Name");
			serial_println("Class");
			serial_println("State");
			serial_println("Suspended Status");
			serial_println("Priority");
			serial_println("");
		  break;
		}

		else if (!strcmpigncase(command, "createPCB"))  {
		  serial_println("The Create PCB command will create a PCB and insert the PCB in the appropriate queue.");
		  break; 
		}

		else if (!strcmpigncase(command, "deletePCB"))  {
		  serial_println("The Delete PCB command will remove a PCB from the appropriate queue and then free all associated memory.");
		  break; 
		}

		else if (!strcmpigncase(command, "block"))  {
		  serial_println("Places a PCB in the blocked state and reinserts it into the appropriate queue.");
		  break; 
		}

		else if (!strcmpigncase(command, "unblock"))  {
		  serial_println("Places a PCB in the unblocked state and reinserts it into the appropriate queue. ");
		  break; 
		}
		

		else {
			serial_print("Unrecognized command: ");
			serial_println(command);
		}
	}
	serial_println("");
}

void setdate()
{
    setyear();
    setmonth();
    setday();

    serial_print("Time successfully changed to -> ");
    getdate();
}

void getdate()
{
	char *encodedweekday = getweekday();
    serial_print(toweekday(encodedweekday));
    serial_print(" ");

    char *encodedmonth = getmonth();
    serial_print(tomonth(encodedmonth));
    serial_print(" ");

    serial_print(getmonthday());
    serial_print(" ");

    char *encodedyear = getyear();
    char *year = sys_alloc_mem(5);
    year[0] = '2';
    year[1] = '0';
    year[2] = encodedyear[0];
    year[3] = encodedyear[1];
    year[4] = '\0';
    serial_println(year);
    serial_println("");
    sys_free_mem(encodedyear);
    sys_free_mem(encodedweekday);
    sys_free_mem(encodedmonth);
    sys_free_mem(year);
}

void settime()
{
	sethour();
    setminute();
    setseconds();

    serial_print("Time successfully changed to -> ");
    gettime();
}

void gettime()
{
    serial_print(gethour());
    serial_print(":");

    serial_print(getminute());
    serial_print(":");

    serial_println(getsecond());
    serial_println("");
}

void suspendPCB()
{
	serial_print("Please enter the name of the process to suspend: ");
	char* name = polling();
	PCB *target = findPCB(name);
	if(target == NULL)
	{
		serial_print("Process not found: \"");
		serial_print(name);
		serial_println("\" is not a valid process name");
	}
	else
	{
		target->suspendedState = 1;
		serial_print("The process \"");
		serial_print(name);
		serial_println("\" has been suspended successfully");
	}
	serial_println("");
}

void resumePCB()
{
	serial_print("Please enter the name of the process to resume: ");
	char* name = polling();
	PCB *target = findPCB(name);
	if(target == NULL)
	{
		serial_print("Process not found: \"");
		serial_print(name);
		serial_println("\" is not a valid process name");
	}
	else
	{
		target->suspendedState = 0;
		serial_print("The process \"");
		serial_print(name);
		serial_println("\" has resumed successfully");
	}
	serial_println("");
}

void setPriority()
{
	serial_print("Please enter the name of the process to alter: ");
	char* name = polling();
	PCB *target = findPCB(name);
	if(target == NULL)
	{
		serial_print("Process not found: \"");
		serial_print(name);
		serial_println("\" is not a valid process name");
	}
	else
	{
		serial_print("What is the new priority? ");
		char* newPriority = polling();
		int priority = atoi(newPriority);
		if(-1 < priority && 10 > priority)
		{
			target->priority = priority;
			int readyIndex = indexByNameReady(name);
			int blockedIndex = indexByNameBlocked(name);
			if (readyIndex >= 0) {
				removePCBAtIndexReady(readyIndex);
			} else {
				removePCBAtIndexBlocked(blockedIndex);
			}
			insertPCB(target);
			serial_print("The process \"");
			serial_print(name);
			serial_println("\" has successfully updated its priority");
		}
		else
		{
			serial_println("Cannot set priority: invalid range");
		}
	}
	serial_println("");
}

void showPCB() {
    serial_print("Please enter the name of the desired PCB to view: ");
    char* pcbName = polling();
    PCB* process = findPCB(pcbName);
    
    if (process == NULL) {
      serial_print(pcbName);
      serial_println(" is not a valid PCB");
      return;
    }
    
    PrintPCB(process);
}

void showReady() {
  PCB* process = peekReady();
  
  while (process != NULL) {
    PrintPCB(process);
    
    process = process->next;
  }
}

void showBlocked() {
  PCB* process = peekBlocked();

  while (process != NULL) {
    PrintPCB(process);

    process = process->next;
  }
}

void showAll() {
  showReady();
  showBlocked();
}

void createPCB()
{
	serial_print("Please provide a name for process being created: ");
    char* pcbName = polling();
    while ((strlen(pcbName) == -1) || indexByNameReady(pcbName) >= 0 || indexByNameBlocked(pcbName) >= 0) {
    	if (strlen(pcbName) == -1) {
    		serial_print("No name was given, please try again: ");
    	} else {
    		serial_print("The given process name is currently in use, please provide another: ");
    	}
   		pcbName = polling();
    }

    serial_print("Is the process an APPLICATION process or a SYSTEM process (0 for APPLICATION / 1 for SYSTEM): ");
    char* inputClass = polling();
    while (strcmpigncase(inputClass, "0") && strcmpigncase(inputClass, "1")) {
    	serial_print("The provided class was not valid, please try again (0 for APPLICATION / 1 for SYSTEM): ");
    	inputClass = polling();
    }

    unsigned char pcbClass = atoi(inputClass);
    serial_print("What is the priority for this process (0-9): ");
    int pcbPriority = atoi(polling());
    while (pcbPriority < 0 || pcbPriority > 9) {
    	serial_print("The provided priority was not valid, please try again (0-9): ");
    	pcbPriority = atoi(polling());
    }
    PCB *createdPCB = SetupPCB(pcbName, pcbClass, pcbPriority);
    insertPCB(createdPCB);
    serial_println("Process successfully created.");
    serial_println("");
}

void deletePCB()
{
	serial_print("Please enter the name of the process to delete: ");
	char* name = polling();
	PCB* target = findPCB(name);
	if(target == NULL)
	{
		if (strlen(name) == 0) {
			serial_print("No name provided exiting command...");
		} else {
			serial_print("Process not found: \"");
			serial_print(name);
			serial_println("\" does not appear to exist");
		}
	}
	else
	{
		int readyIndex = indexByNameReady(name);
		int blockedIndex = indexByNameBlocked(name);
		PCB* removedPCB;
		if (readyIndex >= 0) {
			removedPCB = removePCBAtIndexReady(readyIndex);
		} else {
			removedPCB = removePCBAtIndexBlocked(blockedIndex);
		}
		FreePCB(removedPCB);
		// if (!FreePCB(removedPCB)) {	
			serial_print("The process \"");
			serial_print(name);
			serial_println("\" has been deleted successfully");
		// } else {
		// 	serial_print("An error occurred while attempting to remove the process \"");
		// 	serial_print(name);
		// 	serial_println("\"");
		// }
	}
	serial_println("");
}

void block()
{
	serial_print("Please enter the name of the process to block: ");
	char* name = polling();
	PCB *target = findPCB(name);
	if(target == NULL)
	{
		serial_print("Process not found: \"");
		serial_print(name);
		serial_println("\" is not a valid process name");
	}
	else
	{
		target->readyState = 2;
		int readyIndex = indexByNameReady(name);
		if (readyIndex >= 0) {
			removePCBAtIndexReady(readyIndex);
			insertPCB(target);
			serial_print("The process \"");
			serial_print(name);
			serial_println("\" has been successfully blocked");
		} else {
			serial_println("The process given is already blocked and has not been altered.");
		}
	}
	serial_println("");
}

void unblock()
{
	serial_print("Please enter the name of the process to unblock: ");
	char* name = polling();
	PCB *target = findPCB(name);
	if(target == NULL)
	{
		serial_print("Process not found: \"");
		serial_print(name);
		serial_println("\" is not a valid process name");
	}
	else
	{
		target->readyState = 0;
		int blockedIndex = indexByNameBlocked(name);
		if (blockedIndex >= 0) {
			removePCBAtIndexBlocked(blockedIndex);
			insertPCB(target);
			serial_print("The process \"");
			serial_print(name);
			serial_println("\" has been successfully unblocked");
		} else {
			serial_println("The process given is not blocked and has not been altered.");
		}
	}
	serial_println("");
}

void loadR3()
{

}