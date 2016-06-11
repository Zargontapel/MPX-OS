#include "date_time.h"

void writetoRTC(unsigned char portnum, char *message)
{
    outb(0x70, portnum);
    unsigned char encodedtime;
    if (message[0] == '\0') {
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

char* fromweekday(char *dayname)
{
	char *daynum = sys_alloc_mem(2);
	daynum[0] = '0';
	if(!strcmpigncase(dayname, "sun") || !strcmpigncase(dayname, "sunday")) {
		daynum[1] = '1';
	} else if (!strcmpigncase(dayname, "mon") || !strcmpigncase(dayname, "monday")) {
		daynum[1] = '2';
	} else if (!strcmpigncase(dayname, "tue") || !strcmpigncase(dayname, "tuesday")) {
		daynum[1] = '3';
	} else if (!strcmpigncase(dayname, "wed") || !strcmpigncase(dayname, "wednesday")) {
		daynum[1] = '4';
	} else if (!strcmpigncase(dayname, "thu") || !strcmpigncase(dayname, "thursday")) {
		daynum[1] = '5';
	} else if (!strcmpigncase(dayname, "fri") || !strcmpigncase(dayname, "friday")) {
		daynum[1] = '6';
	} else if (!strcmpigncase(dayname, "sat") || !strcmpigncase(dayname, "saturday")) {
		daynum[1] = '7';
	} else {
		daynum[0] = '\0';
	}
	return daynum;
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

char* frommonth(char *monthname)
{
	char *monthnum = sys_alloc_mem(2);
	monthnum[0] = '0';
	if(!strcmpigncase(monthname, "jan") || !strcmpigncase(monthname, "january")) {
		monthnum[1] = '1';
	} else if (!strcmpigncase(monthname, "feb") || !strcmpigncase(monthname, "february")) {
		monthnum[1] = '2';
	} else if (!strcmpigncase(monthname, "mar") || !strcmpigncase(monthname, "march")) {
		monthnum[1] = '3';
	} else if (!strcmpigncase(monthname, "apr") || !strcmpigncase(monthname, "april")) {
		monthnum[1] = '4';
	} else if (!strcmpigncase(monthname, "may")) {
		monthnum[1] = '5';
	} else if (!strcmpigncase(monthname, "jun") || !strcmpigncase(monthname, "june")) {
		monthnum[1] = '6';
	} else if (!strcmpigncase(monthname, "jul") || !strcmpigncase(monthname, "july")) {
		monthnum[1] = '7';
	} else if (!strcmpigncase(monthname, "aug") || !strcmpigncase(monthname, "august")) {
		monthnum[1] = '8';
	} else if (!strcmpigncase(monthname, "sep") || !strcmpigncase(monthname, "september")) {
		monthnum[1] = '9';
	} else if (!strcmpigncase(monthname, "oct") || !strcmpigncase(monthname, "october")) {
		monthnum[0] = '1';
		monthnum[1] = '0';
	} else if (!strcmpigncase(monthname, "nov") || !strcmpigncase(monthname, "november")) {
		monthnum[0] = '1';
		monthnum[1] = '1';
	} else if (!strcmpigncase(monthname, "dec") || !strcmpigncase(monthname, "december")) {
		monthnum[0] = '1';
		monthnum[1] = '2';
	} else {
		monthnum[0] = '\0';
	}
	return monthnum;
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