#include "com_handler.h"

void comhandle()
{
	serial_println("");
	serial_println("Welcome to the MPX OS.");
	serial_println("Feel free to begin entering commands.");
	serial_println("");
	while(1) {
		char *command = polling();
		if (!strcmp(command, "shutdown")) {
			if (shutdownConfirmed()) {
				serial_println("System shutting down...");
				break;
			} else {
				serial_println("Shutdown canceled.");
			}
		} else if (!strcmp(command, "version")) {
			version();
		} else if (!strcmp(command, "help")) {
			help();
		} else if (!strcmp(command, "setdate")) {
			setdate();
		} else if (!strcmp(command, "getdate")) {
			getdate();
		} else if (!strcmp(command, "settime")) {
			settime();
		} else if (!strcmp(command, "gettime")) {
			gettime();
		} else if (strcmp(command, '\0')) {
			serial_println("Command not recognized. Type help to view commands.");
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
	serial_println("Version 1.0 Module R1");
	serial_println("Completion Date: Thursday, February 5, 2015");
	serial_println("");
}

void help()
{
	serial_println("No worries, I have you covered.");
    serial_println("Here's a list of available commands:");
    serial_println("version     shows the current version of the OS");
    serial_println("help        you're doing it right now");
    serial_println("setdate     allows user to set the date via prompts");
    serial_println("getdate     display the current system date");
    serial_println("settime     allows user to set the system time");
    serial_println("gettime     displays the current system time");
    serial_println("shutdown    turns off the system by exiting the kernel");
    serial_println("Alright, now go get 'em!");
    serial_println("");
}

void setdate()
{
    serial_println("What year is it?");
    char *response = polling();
    char *bcdyear = sys_alloc_mem(2);
    if (strlen(response) != 4 || atoi(response) < 2000) {
        bcdyear[0] = '\0';
    } else {
        bcdyear[0] = response[2];
        bcdyear[1] = response[3];
    }
    unsigned char portnum = 0x09;
    writetoRTC(portnum, bcdyear);

    serial_println("What month is it?");
    response = polling();
  	char *bcdmonth = frommonth(response);
    portnum = 0x08; //month port
  	writetoRTC(portnum, bcdmonth);

    serial_println("What day of the month is it?");
    response = polling();
    portnum = 0x07; //month day port
  	writetoRTC(portnum, response);

    serial_print("Time successfully changed to -> ");
    getdate();
    sys_free_mem(response);
    sys_free_mem(bcdyear);
    sys_free_mem(bcdmonth);
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
