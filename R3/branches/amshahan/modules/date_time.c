#include "date_time.h"

void writetoRTC(unsigned char portnum, char *message)
{
    outb(0x70, portnum);
    unsigned char encodedtime;
    if (message == '\0') {
        encodedtime = inb(0x71);
    } else {
        encodedtime = toBCD(message);
    }
    cli();
    outb(0x71, encodedtime);
    sti();
}

void fromBCD(unsigned char bcd, char *convertedchar)
{
	*convertedchar++ = (bcd >> 4) + '0';
	*convertedchar++ = (bcd & 0x0f) + '0';
	*convertedchar = '\0';
}

unsigned char toBCD(char *chartoconvert)
{
	if(strlen(chartoconvert) == 1) {
		chartoconvert[1] = chartoconvert[0];
		chartoconvert[0] = '0';
	}
	unsigned char bcd;
	bcd = (chartoconvert[0] - '0') << 4;
	bcd |= (chartoconvert[1] - '0');
	return bcd;
}

char* toweekday(char *daynum)
{
	char *dayname = sys_alloc_mem(4);
	switch(daynum[1]) {
		case '1':
			dayname = "Sun";
			break;
		case '2':
			dayname = "Mon";
			break;
		case '3':
			dayname = "Tue";
			break;
		case '4':
			dayname = "Wed";
			break;
		case '5':
			dayname = "Thu";
			break;
		case '6':
			dayname = "Fri";
			break;
		case '7':
			dayname = "Sat";
			break;
	}
	return dayname;
}

char* tomonth(char *monthnum)
{
	char *monthname = sys_alloc_mem(4);
	if(!strcmp(monthnum, "01")) {
		monthname = "Jan";
	} else if (!strcmp(monthnum, "02")) {
		monthname = "Feb";
	} else if (!strcmp(monthnum, "03")) {
		monthname = "Mar";
	} else if (!strcmp(monthnum, "04")) {
		monthname = "Apr";
	} else if (!strcmp(monthnum, "05")) {
		monthname = "May";
	} else if (!strcmp(monthnum, "06")) {
		monthname = "Jun";
	} else if (!strcmp(monthnum, "07")) {
		monthname = "Jul";
	} else if (!strcmp(monthnum, "08")) {
		monthname = "Aug";
	} else if (!strcmp(monthnum, "09")) {
		monthname = "Sep";
	} else if (!strcmp(monthnum, "10")) {
		monthname = "Oct";
	} else if (!strcmp(monthnum, "11")) {
		monthname = "Nov";
	} else if (!strcmp(monthnum, "12")) {
		monthname = "Dec";
	}
	return monthname;
}

int isvalid(int num, int lowerbound, int upperbound)
{
	return (num >= lowerbound && num <= upperbound);
}

int isvalidyear(int year)
{
	return (isvalid(year, 2000, 2099) || isvalid(year, 0, 99));
}

int isleapyear(int year)
{
	return (year % 4) || ((year % 100 == 0) && (year % 400)) ? 0 : 1;
}

int isvalidmonth(int month)
{
	return (isvalid(month, 1, 12));
}

int isvalidday(int day)
{
	date_time date;
	date.year = atoi(getyear());
	date.year += 2000;
	date.mon = atoi(getmonth());
	int validDay;
	if (date.mon == 1) {
		validDay = isvalid(day, 1, 31);
	} else if (date.mon == 2 && isleapyear(date.year)) {
		validDay = isvalid(day, 1, 29);
	} else if (date.mon == 2) {
		validDay = isvalid(day, 1, 28);
	} else if (date.mon == 3) {
		validDay = isvalid(day, 1, 31);
	} else if (date.mon == 4) {
		validDay = isvalid(day, 1, 30);
	} else if (date.mon == 5) {
		validDay = isvalid(day, 1, 31);
	} else if (date.mon == 6) {
		validDay = isvalid(day, 1, 30);
	} else if (date.mon == 7) {
		validDay = isvalid(day, 1, 31);
	} else if (date.mon == 8) {
		validDay = isvalid(day, 1, 31);
	} else if (date.mon == 9) {
		validDay = isvalid(day, 1, 30);
	} else if (date.mon == 10) {
		validDay = isvalid(day, 1, 31);
	} else if (date.mon == 11) {
		validDay = isvalid(day, 1, 30);
	} else {
		validDay = isvalid(day, 1, 31);
	}
	return validDay;
}

int isvalidhour(int hour)
{
	return (isvalid(hour, 0, 23));
}

int isvalidminute(int minute)
{
	return (isvalid(minute, 0, 59));
}

int isvalidseconds(int seconds)
{
	return (isvalid(seconds, 0, 59));
}

void setyear()
{
	serial_println("What year is it?");
    char *response = polling();
    char *bcdyear = sys_alloc_mem(2);
    int year = atoi(response);
    while (!(isNumeric(response) && isvalidyear(year)) && response != '\0') {
        serial_println("Year entered was not recognized. Please try again (yy or yyyy).");
        response = polling();
        year = atoi(response);
    }
    if (strlen(response) == 4) {
        bcdyear[0] = response[2];
        bcdyear[1] = response[3];
    } else {
        bcdyear = response;
    }
    unsigned char portnum = 0x09;
    writetoRTC(portnum, bcdyear);
    sys_free_mem(response);
    sys_free_mem(bcdyear);
}

void setmonth()
{
	serial_println("What month is it?");
    char *response = polling();
    int month = atoi(response);
    while (!isvalidmonth(month) && response != '\0') {
        serial_println("Month entered was not recognized. Please try again (mm).");
        response = polling();
        month = atoi(response);
    }
    unsigned char portnum = 0x08; //month port
  	writetoRTC(portnum, response);
  	sys_free_mem(response);
}

void setday()
{
	serial_println("What day of the month is it?");
    char *response = polling();
    int day = atoi(response);
    while (!isvalidday(day) && response != '\0') {
        serial_println("Day entered was not valid. Please try again (dd).");
        response = polling();
        day = atoi(response);
    }
    unsigned char portnum = 0x07; //month day port
  	writetoRTC(portnum, response);
  	sys_free_mem(response);
}

void sethour()
{
	serial_println("What is the current hour?");
    char *response = polling();
    int hour = atoi(response);
    while (!(isNumeric(response) && isvalidhour(hour)) && response != '\0') {
    	serial_println("Hour entered was not valid. Please try again (HH).");
        response = polling();
        hour = atoi(response);
    }
    unsigned char portnum = 0x04; //hours port
    writetoRTC(portnum, response);
    sys_free_mem(response);
}

void setminute()
{
	serial_println("What is the current minute?");
    char *response = polling();
    int minute = atoi(response);
    while (!(isNumeric(response) && isvalidminute(minute)) && response != '\0') {
    	serial_println("Minute entered was not valid. Please try again (MM).");
        response = polling();
        minute = atoi(response);
    }
    unsigned char portnum = 0x02; //minutes port
    writetoRTC(portnum, response);
    sys_free_mem(response);
}

void setseconds()
{
	serial_println("What is the current second?");
    char *response = polling();
    int seconds = atoi(response);
    while (!(isNumeric(response) && isvalidseconds(seconds)) && response != '\0') {
    	serial_println("Seconds entered were not valid. Please try again (SS).");
        response = polling();
        seconds = atoi(response);
    }
    unsigned char portnum = 0x00; //seconds port
    writetoRTC(portnum, response);
    sys_free_mem(response);
}

char* getweekday()
{
	return getdatetimeunit(0x06);
}

char* getmonth()
{
	return getdatetimeunit(0x08);
}

char* getmonthday()
{
	return getdatetimeunit(0x07);
}

char* getyear()
{
	return getdatetimeunit(0x09);
}

char* gethour()
{
	return getdatetimeunit(0x04);
}

char* getminute()
{
	return getdatetimeunit(0x02);
}

char* getsecond()
{
	return getdatetimeunit(0x00);
}

char* getdatetimeunit(unsigned char port)
{
	outb(0x70, port);
	unsigned char beforeconv = inb(0x71);
	char *afterconv = sys_alloc_mem(5);
	fromBCD(beforeconv, afterconv);
	return afterconv;
}