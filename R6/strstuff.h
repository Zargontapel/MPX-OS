#ifndef _STRING_H
#define _STRING_H



/*
  Procedure..: strcmpigncase
  Description..: String comparison ignoring cases
  Params..: s1-string 1, s2-string 2
*/
int strcmpigncase(const char *s1, const char *s2);
int endsWith(const char *str, const char *suffix);
void toUpper(char*);

#endif
