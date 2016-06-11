#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_PATH_SIZE 10
#define MAX_NAME_LENGTH 8
#define MAX_EXT_LENGTH 3

int shutdownConfirmed();
void bootinfo();
void printroot();
void cd(char*, FILE*, uint16_t[]);
void list(char*);
void type(uint16_t[], char*);
void relabel(char*, char*, FILE*, uint16_t[]);
char** tokenizeName(char*);
void help();
