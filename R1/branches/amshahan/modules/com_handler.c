#include "com_handler.h"

void comhandle()
{
	serial_println("");
	serial_println("Welcome to the MPX OS.");
	serial_println("Feel free to begin entering commands.");
	serial_println("");
	while(1) {
		char *command = polling();
		if (!strcmpigncase(command, "Shutdown")) {
			if (shutdownConfirmed()) {
				serial_println("System shutting down...");
				break;
			} else {
				serial_println("Shutdown canceled.");
			}
		} else if (!strcmpigncase(command, "Version")) {
			version();
		} else if (!strcmpigncase(command, "Help")) {
			help();
		} else if (!strcmpigncase(command, "Setdate")) {
			setdate();
		} else if (!strcmpigncase(command, "Getdate")) {
			getdate();
		} else if (!strcmpigncase(command, "Settime")) {
			settime();
		} else if (!strcmpigncase(command, "Gettime")) {
			gettime();
		}
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
	serial_println("Version R1");
	serial_println("Completion Date: Thursday, February 5, 2015");
	serial_println("");
}

void help()
{
  while(1) {
    serial_println("Which command would you like to know more about?");
    serial_println("Enter CommandList for a list of commands.");
    serial_println("Enter QuitHelp to exit.");
    char *command = polling();
    if (!strcmpigncase(command, "CommandList")) {
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
    else if (!strcmpigncase(command, "QuitHelp")) {
      return;
    }
    else if (!strcmpigncase(command, "Version")) {
      serial_println("Version shows the current version of the operating system.");
      return;
    }
    else if (!strcmpigncase(command, "Help")) {
      serial_println("Help gets further help for a prompted command");
      return;
    }
    else if (!strcmpigncase(command, "Setdate")) {
      serial_println("Setdate allows the user to set the date of the operating system.");
      return;
    }
    else if (!strcmpigncase(command, "Getdate")) {
      serial_println("Getdate returns the current date of the operating system.");
      return;
    }
    else if (!strcmpigncase(command, "Settime")) {
      serial_println("Settime allows the user to set the time of the operating system.");
      return;
    }
    else if (!strcmpigncase(command, "Gettime")) {
      serial_println("Gettime returns the current date of the operating system.");
      return;
    }
    else if (!strcmpigncase(command, "Shutdown")) {
      serial_println("Shutdown turns off the operating system.");
      return;
    }
    else {
      serial_print("Unrecognized command: ");
      serial_println(command);
    }
  }    
}

void setdate()
{
	serial_println("What day of the week is it?");
    char *response = polling();
  	char *bcdweekday = fromweekday(response);
    unsigned char portnum = 0x06; //weekday port
    writetoRTC(portnum, bcdweekday);

    serial_println("What month is it?");
    response = polling();
  	char *bcdmonth = frommonth(response);
    portnum = 0x08; //month port
  	writetoRTC(portnum, bcdmonth);

    serial_println("What day of the month is it?");
    response = polling();
    portnum = 0x07; //month day port
  	writetoRTC(portnum, response);

    serial_println("What year is it?");
    response = polling();
  	char *bcdyear = sys_alloc_mem(2);
  	if (strlen(response) != 4 || atoi(response) < 2000) {
  		bcdyear[0] = '\0';
  	} else {
  		bcdyear[0] = response[2];
  		bcdyear[1] = response[3];
  	}
    portnum = 0x09;
    writetoRTC(portnum, bcdyear);

    serial_print("Time successfully changed to -> ");
    getdate();
    sys_free_mem(response);
    sys_free_mem(bcdweekday);
    sys_free_mem(bcdmonth);
    sys_free_mem(bcdyear);
}

void getdate()
{
	outb(0x70, 0x06);
	unsigned char beforeconv = inb(0x71);
	char *afterconv = sys_alloc_mem(5);
	fromBCD(beforeconv, afterconv);
	char *weekday = toweekday(afterconv);
    serial_print(weekday);
    serial_print(" ");

    outb(0x70, 0x08);
    beforeconv = inb(0x71);
    fromBCD(beforeconv, afterconv);
    char *month = tomonth(afterconv);
    serial_print(month);
    serial_print(" ");

    outb(0x70, 0x07);
    beforeconv = inb(0x71);
    fromBCD(beforeconv, afterconv);
    serial_print(afterconv);
    serial_print(" ");

    outb(0x70, 0x09);
    beforeconv = inb(0x71);
    fromBCD(beforeconv, afterconv);
    char *year = sys_alloc_mem(5);
    year[0] = '2';
    year[1] = '0';
    year[2] = afterconv[0];
    year[3] = afterconv[1];
    year[4] = '\0';
    serial_println(year);
    serial_println("");
    sys_free_mem(afterconv);
    sys_free_mem(weekday);
    sys_free_mem(month);
    sys_free_mem(year);
}

void settime()
{
	serial_println("What is the current hour?");
    char *response = polling();
    unsigned char portnum = 0x04; //hours port
    writetoRTC(portnum, response);

    serial_println("What is the current minute?");
    response = polling();
    portnum = 0x02; //minutes port
    writetoRTC(portnum, response);

    serial_println("What is the current second?");
    response = polling();
    portnum = 0x00; //seconds port
    writetoRTC(portnum, response);

    serial_print("Time successfully changed to -> ");
    gettime();
    sys_free_mem(response);
}

void gettime()
{
	outb(0x70, 0x04);
	unsigned char bcdtime = inb(0x71);
	char *convtime = sys_alloc_mem(5);
	fromBCD(bcdtime, convtime);
    serial_print(convtime);
    serial_print(":");

    outb(0x70, 0x02);
    bcdtime = inb(0x71);
    fromBCD(bcdtime, convtime);
    serial_print(convtime);
    serial_print(":");

    outb(0x70, 0x00);
    bcdtime = inb(0x71);
    fromBCD(bcdtime, convtime);
    serial_println(convtime);
    serial_println("");
    sys_free_mem(convtime);
}
