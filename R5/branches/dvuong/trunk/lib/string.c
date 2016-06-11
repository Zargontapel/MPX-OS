#include <system.h>
#include <string.h>

/* ****************************** *
 * ********* Your Task: ********* *
 * ******** complete the ******** *
 * * following string functions * *
 * ****************************** */

/*
  Procedure..: strlen
  Description..: Returns the length of a string.
  Params..: s-input string
*/
int strlen(const char *s)
{
  const char *loc = s;

  //check for empty string
  if (loc == NULL)
    return -1;
  
  while (*loc != '\0') {
    loc++;
  }

  return (loc-s);
}

/*
  Procedure..: strcpy
  Description..: Copy one string to another.
  Params..: s1-destination, s2-source
*/
char* strcpy(char *s1, const char *s2)
{
  int count;
  for (count = 0; s2[count] != '\0'; count++) {
    s1[count] = s2[count];
  }
  s1[count] = '\0';
  return s1; // return pointer to destination string
}

/*
  Procedure..: atoi
  Description..: Convert an ASCII string to an integer
  Params..: const char *s -- String
*/
int atoi(const char *s)
{
  //eliminate white space at front of string
  while (isspace(s)) {
    s++;
  }

  int num = 0;
  int sign = 1;
  //set integer sign
  if (*s == '-') {
    sign = -1;
    s++;
  } else if (*s == '+') {
    s++;
  }

  //create number
  for ( ; isNumeric(s); s++) {
    num = num * 10 + *s - '0';
  }
  return num * sign; // return integer after adjusting sign
}

/*
  Procedure..: isNumeric
  Description..: Helper function for usage in other methods. Determines if
    character is representative of value 0-9
  Params..: c-character to determine is numeric
*/
int isNumeric(const char *c)
{
  return (*c >= '0' && *c <= '9');
}

/*
  Procedure..: strcmp
  Description..: String comparison
  Params..: s1-string 1, s2-string 2
*/
int strcmp(const char *s1, const char *s2)
{
  for ( ; *s1 == *s2 && *s1 != '\0' && *s2 != '\0'; s1++, s2++);
  // Remarks:
  // 1) If we made it to the end of both strings (i. e. our pointer points to a
  //    '\0' character), the function will return 0
  // 2) If we didn't make it to the end of both strings, the function will
  //    return the difference of the characters at the first index of
  //    indifference.
  return ( *(unsigned char *)s1 - *(unsigned char *)s2 );
}

/*
  Procedure..: tolower
  Description..: Convert char to lower case from upper case
  Params..: c-character to convert
*/
int tolower(int c)
{
  if (c >= 'A' && c <= 'Z') {
    c = 'a' + (c - 'A');
  }
  return c;
}

/*
  Procedure..: strcmpigncase
  Description..: String comparison ignoring cases
  Params..: s1-string 1, s2-string 2
*/
int strcmpigncase(const char *s1, const char *s2)
{
    do {
        int testValue = tolower(s1[0]) - tolower(s2[0]);
        if (testValue != 0 || !s1[0]) {
            return testValue;
        }
        s1++, s2++;
    } while (*s1 != '\0' && *s2 != '\0');
    
    return tolower(s1[0]) - tolower(s2[0]);
}

/* And finally....
   For the brave ones! (Note: you'll need to add a prototype to string.h)
   sprintf must work properly for the following types to receive extra credit:
     1) characters
     2) strings
     3) signed integers
     4) hexadecimal numbers may be useful
     ...
     \infty) Or feel free to completely implement sprintf
             (Read the man Page: $ man sprintf)
   int sprintf(char *str, const char *format, ...); 
*/


/* ---------------------------------------
    Functions below this point are given.
    No need to tamper with these!
   --------------------------------------- */

/*
  Procedure..: strcat
  Description..: Concatenate the contents of one string onto another.
  Params..: s1-destination, s2-source
*/
char* strcat(char *s1, const char *s2)
{
  char *rc = s1;
  if (*s1) while(*++s1);
  while( (*s1++ = *s2++) );
  return rc;
}

/*
  Procedure..: isspace
  Description..: Determine if a character is whitespace.
  Params..: c-character to check
*/
int isspace(const char *c)
{
  if (*c == ' '  ||
      *c == '\n' ||
      *c == '\r' ||
      *c == '\f' ||
      *c == '\t' ||
      *c == '\v'){
    return 1;
  }
  return 0;
}

/*
  Procedure..: memset
  Description..: Set a region of memory.
  Params..: s-destination, c-byte to write, n-count
*/
void* memset(void *s, int c, size_t n)
{
  unsigned char *p = (unsigned char *) s;
  while(n--){
    *p++ = (unsigned char) c;
  }
  return s;
}

/*
  Procedure..: strtok
  Description..: Split string into tokens
  Params..: s1-string, s2-delimiter
*/
char* strtok(char *s1, const char *s2)
{
  static char *tok_tmp = NULL;
  const char *p = s2;

  //new string
  if (s1!=NULL){
    tok_tmp = s1;
  }
  //old string cont'd
  else {
    if (tok_tmp==NULL){
      return NULL;
    }
    s1 = tok_tmp;
  }

  //skip leading s2 characters
  while ( *p && *s1 ){
    if (*s1==*p){
      ++s1;
      p = s2;
      continue;
    }
    ++p;
  }

  //no more to parse
  if (!*s1){
    return (tok_tmp = NULL);
  }

  //skip non-s2 characters
  tok_tmp = s1;
  while (*tok_tmp){
    p = s2;
    while (*p){
      if (*tok_tmp==*p++){
	*tok_tmp++ = '\0';
	return s1;
      }
    }
    ++tok_tmp;
  }

  //end of string
  tok_tmp = NULL;
  return s1;
}

char* itoa(int num)
{
	static char buf[21];
	char *p = buf + 20;
	if (num >= 0)
	{
		do{
			*--p = '0' + (num % 10);
			num /= 10;
		}while (num != 0);
		return p;
	}
	else
	{
		do{
			*--p = '0' - (num % 10);
			num /= 10;
		}while (num != 0);
		*--p = '-';
	}
	return p;
}
