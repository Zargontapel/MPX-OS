#include <string.h>
#include <stdlib.h>

/* ****************************** *
 * ********* Your Task: ********* *
 * ******** complete the ******** *
 * * following string functions * *
 * ****************************** */


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

//converts a string to all upper case, in keeping with DOS tradition
void toUpper(char* string)
{
  int i = 0;
  for(; i <= strlen(string); i++)
  {
    string[i] = toupper(string[i]);
  }
}

//determines if a string ends with a given character
int endsWith(const char *str, const char *suffix)
{
    if (!str || !suffix)
        return 0;
    size_t lenstr = strlen(str);
    size_t lensuffix = strlen(suffix);
    if (lensuffix >  lenstr)
        return 0;
    return strncmp(str + lenstr - lensuffix, suffix, lensuffix) == 0;
}
