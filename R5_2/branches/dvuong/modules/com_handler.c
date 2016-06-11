#include "com_handler.h"
#include "pcbmain.h"
#include "r5.h"
#include "string.h"

void comhandle()
{
	serial_println("");
	serial_println("Welcome to the MPX OS.");
	serial_println("Feel free to begin entering commands.");
	serial_println("");
	while(1) {
		sys_req(IDLE);
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
		} else if (!strcmpigncase(command, "showPCB")) {
		        showPCB();
		} else if (!strcmpigncase(command, "showReady")) {
		        showReady();
		} else if (!strcmpigncase(command, "showBlocked")) {
		        showBlocked();
		} else if (!strcmpigncase(command, "showAll")) {
		        showAll();
		} else if (!strcmpigncase(command, "loadr3")) {
		        loadR3();
		} else if (!strcmpigncase(command, "initMemory")) {
			heapInitialize(1000);	
		} else if (!strcmpigncase(command, "allocate")) {
			sys_free_mem(allocateMem());
		} else if (!strcmpigncase(command, "freeMemory")) {
			freeMem();
		} else if (!strcmpigncase(command, "printAlloc")) {
			printAlloc();
		} else if (!strcmpigncase(command, "printFree")) {
			printFree();
		} else if (!strcmpigncase(command, "isEmpty")) {
			isEmpty();
		} else if (strcmp(command, '\0')) {
			serial_println("Command not recognized. Type help to view commands.");
			serial_println("");
		}
		sys_free_mem(command);
	}
	sys_req(EXIT);
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
	serial_println("Version 1.0 Module R4");
	serial_println("Completion Date: Thursday, March 19, 2015");
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
			serial_println("Loadr3");
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

		else if (!strcmpigncase(command, "loadr3")) {
		  serial_println("Loads all r3 processes into memory in a ready blocked state. Used for testing of multiprogramming functionality.");
		  break;
		}

		else {
			serial_print("Unrecognized command: ");
			serial_println(command);
		}
		sys_free_mem(command);
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
	sys_free_mem(name);
	sys_free_mem(target);
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
	sys_free_mem(name);
	sys_free_mem(target);
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
		sys_free_mem(newPriority);
	}
	sys_free_mem(name);
	sys_free_mem(target);
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
    sys_free_mem(pcbName);
    sys_free_mem(process);
}

void showReady() {
  PCB* process = peekReady();
  
  while (process != NULL) {
    PrintPCB(process);
    
    process = process->next;
  }
  sys_free_mem(process);
}

void showBlocked() {
  PCB* process = peekBlocked();

  while (process != NULL) {
    PrintPCB(process);

    process = process->next;
  }
  sys_free_mem(process);
}

void showAll() {
  showReady();
  showBlocked();
}

void loadR3()
{
	void (*func1)() = &proc1;
	loadBasicFunctionToPCB("proc1", 0, 9, 0, 1, 512, func1);
	sys_free_mem(func1);
	void (*func2)() = &proc2;
	loadBasicFunctionToPCB("proc2", 0, 9, 0, 1, 512, func2);
	sys_free_mem(func2);
	void (*func3)() = &proc3;
	loadBasicFunctionToPCB("proc3", 0, 9, 0, 1, 512, func3);
	sys_free_mem(func3);
	void (*func4)() = &proc4;
	loadBasicFunctionToPCB("proc4", 0, 9, 0, 1, 512, func4);
	sys_free_mem(func4);
	void (*func5)() = &proc5;
	loadBasicFunctionToPCB("proc5", 0, 9, 0, 1, 512, func5);
	sys_free_mem(func5);
    serial_println("Processes successfully created.");
    serial_println("");
}

void loadBasicFunctionToPCB(char name[], unsigned char class, unsigned char priority, unsigned char readyState, unsigned char suspendedState, int stackSize, void (*func)())
{
	PCB* proc = SetupPCB(name, class, priority, readyState, suspendedState, stackSize);
	if (proc != NULL) {
		context *newContext = (context*)(proc->stackTop);
		memset(newContext, 0, sizeof(context));
		newContext->fs = 0x10;
		newContext->gs = 0x10;
		newContext->ds = 0x10;
		newContext->es = 0x10;
		newContext->cs = 0x8;
		newContext->ebp = (u32int)(proc->stackBase);
		newContext->esp = (u32int)(proc->stackTop);
		newContext->eip = (u32int)func;
		newContext->eflags = 0x202;
		insertPCB(proc);
	}
	sys_free_mem(proc);
}

void heapInitialize(int size)
{
	int status = init_heap(size);
	if(status == -1)
	{
		serial_println("Error: heap not initialized");
	}
	else
	{
		serial_print("Success! Heap initialized to ");
		serial_print(itoa(status));
		serial_println(" bytes.");
	}
}

void* allocateMem()
{
	serial_println("How many bytes do you need to allocate? ");
	char* size = polling();
	u32int ptr = my_alloc(atoi(size));
	serial_print("The allocated memory is ");
	serial_println(itoa(ptr));
	if(ptr != NULL)
	{
		CompleteMCB* pointer = (CompleteMCB*)ptr;
		serial_println(itoa(pointer->beginningAddress));
		return pointer;
	}
	else
	{
		serial_println("ERROR: Memory allocation failed.");
		return NULL;
	}
	sys_free_mem(size);
}

void freeMem()
{
	serial_println("Please enter the address of the block to free: ");
	char* addr = polling();
	int result = my_free((void *)addr);
	if(result == -1)
	{
		serial_print("Successfully freed memory at location ");
		serial_println(addr);
	}
	else
	{
		serial_println("ERROR: Memory could not be freed.");
	}
	sys_free_mem(addr);
}

void printAlloc()
{
	print_alloc();
}

void printFree()
{
	print_free();
}

void isEmpty()
{
	int result = is_empty();
	if(result == 1)
	{
		serial_println("Yep! It's empty.");
	}
	else
	{
		serial_println("It's full of stuff.");
	}
}
