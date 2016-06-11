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
		} else if (!strcmpigncase(command, "showPCB")) {
		        showPCB();
		} else if (!strcmpigncase(command, "showReady")) {
		        showReady();
		} else if (!strcmpigncase(command, "showBlocked")) {
		        showBlocked();
		} else if (!strcmpigncase(command, "showAll")) {
		        showAll();
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
	serial_println("Work in Progress");
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

void showPCB() {
    serial_println("Enter the PCB name");
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
