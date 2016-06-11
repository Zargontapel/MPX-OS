#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <com_handler.h>

#define BOOT_SECTOR_SIZE 512
#define ROOT_DIR_SIZE 224

typedef struct {
	int bytesPerSector;
	int sectorsPerCluster;
	int reservedSectors;
	int fatCopies;
	int maxRootDirEntries;
	int numSectors;
	int sectorsPerFAT;
	int sectorsPerTrack;
	int numHeads;
	int sectorCount;
	int bootSig;
	int volumeID;
	char *volumeLabel;
	char *fileSystemType;
} BootSector;

typedef struct {
	char* fileName;
	char* extension;
	unsigned char attributes;
	unsigned int reservedInfo;
	unsigned int creationTime;
	unsigned int creationDate;
	unsigned int lastAccess;
	unsigned int unusedInfo;
	unsigned int lastWriteTime;
	unsigned int lastWriteDate;
	unsigned int firstLogicalCluster;
	unsigned long fileSize;
} directoryEntry;

typedef struct {
	directoryEntry* entry;
	unsigned int entryLocation;
} enhancedDirectoryEntry;

BootSector sector;
enhancedDirectoryEntry* currentDir[ROOT_DIR_SIZE];
enhancedDirectoryEntry* rootDir[ROOT_DIR_SIZE];
unsigned char buffer[BOOT_SECTOR_SIZE];
size_t bytes_read;
FILE *diskImage;
void loadNewDirectory(int, FILE *, uint16_t[]);
void loadRootDirectory(FILE *);
void readSectorEntries(FILE *, int, enhancedDirectoryEntry*[], int);
enhancedDirectoryEntry* readEntry(FILE *);
int writeEntry(FILE *, enhancedDirectoryEntry*);
int changeDir(char*, FILE*, uint16_t[]);
int isDirectory(directoryEntry*);
int logicalToPhysical(int);
int getNextSector(int, uint16_t[]);
char *trimwhitespace(char *str);
char *padString(char *, int);
void printSectorData(int);
void printFAT(uint16_t[]);
char** getTargetPath(char*);
