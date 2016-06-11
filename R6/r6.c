#include "r6.h"

//main, the starting point of our program
int main(int argc, char *argv[])
{
	// read in the image into an array
	diskImage = fopen(argv[1], "r+");
	if(diskImage != NULL)
	{
		bytes_read = fread(buffer, sizeof(unsigned char), BOOT_SECTOR_SIZE, diskImage);
		//printf("Bytes read = %zu\n", bytes_read);
		rewind(diskImage);
		sector.bytesPerSector = (buffer[12] << 8) + buffer[11]; //get number of bytes per sector from array
		sector.sectorsPerCluster = buffer[13]; 
		sector.reservedSectors = (buffer[15] << 8) + buffer[14];
		sector.fatCopies = buffer[16];
		sector.maxRootDirEntries = (buffer[18] << 8) + buffer[17];
		sector.numSectors = (buffer[20] << 8) + buffer[19]; //concatenate the bytes to get number of sectors
		sector.sectorsPerFAT = (buffer[23] << 8) + buffer[22];
		sector.sectorsPerTrack = (buffer[25] << 8) + buffer[24];
		sector.numHeads = (buffer[27] << 8) + buffer[26];
		sector.sectorCount = (buffer[35] << 24) + (buffer[34] << 16) + (buffer[33] << 8) + buffer[32];
		sector.bootSig = buffer[38];
		sector.volumeID = (buffer[42] << 24) + (buffer[41] << 16) + (buffer[40] << 8) + buffer[39];
		sector.volumeLabel = malloc(11);
		sector.fileSystemType = malloc(8);
		uint16_t FAT[sector.sectorsPerFAT * sector.bytesPerSector * 8/12];
		//printf("FAT array size is: %u\n", sector.sectorsPerFAT * sector.bytesPerSector * 8/12);
		fseek(diskImage, 512, SEEK_SET); //go to the start of the FAT on disk
		int i = 0;
		//for(i; i < sizeof(FAT)/16; i+=2)
		for(i; i < sizeof(FAT)/2; i += 2)
		{
			unsigned char temp[3];
			fread(temp, 1, 3, diskImage);
			FAT[i] = (temp[1] << 8) + temp[0];
			FAT[i] &= 4095;
			FAT[i + 1] = (temp[2] << 4) + (temp[1] >> 4);
			FAT[i + 1] &= 4095; 
		}
		//printFAT(FAT);
		i = 43;
		for(i; i < 54; i++)
		{
			sector.volumeLabel[i - 43] = buffer[i];
		}
		i = 54;
		for(i; i < 62; i++)
		{
			sector.fileSystemType[i - 54] = buffer[i];
		}
		loadRootDirectory(diskImage);
		if(argc == 2)
		{
			commandLine(diskImage, FAT);
		}
		else if (argc == 3)
		{
			//dump the file to screen
			
			
		char* target = malloc(100);
		target = argv[2];

		char **targetPath = getTargetPath(target);
		int i = 0;
		for (; i < MAX_PATH_SIZE && (targetPath[i + 1] != NULL); i++)
		{
			if (changeDir(targetPath[i], diskImage, FAT) != 0)
			{
				printf("The specified target path was incorrect and could not be found.\n");
				return;
			}
		}
	char* name = targetPath[i];
	strtok(name, ".");

	//printf("Now printing a file of type .c .txt or .bat\n");
	i = 0;
	int fileFound = 0;
	int value;
	for(i; i < 224; i++)
	{
		if(strcmp(currentDir[i]->entry->fileName, name) == 0)
		{
			int sector = currentDir[i]->entry->firstLogicalCluster;
			char* entry = malloc(1);
			do
			{
				printSectorData(sector);
				//printf("\nPress enter to continue.\n");
				//fgets(entry, 32, stdin);
				sector = FAT[sector];
				value = sector;
			}while(FAT[sector] < 0xFF0);
			printSectorData(sector);
			//printf("\n");
			fileFound = 1;
			break;
		}
	}
	if(!fileFound)
	printf("File not found.\n");

		}
		else
		{
			printf("ERROR: Incorrect number of command line arguments. Exiting...\n");
		}
		free(sector.volumeLabel);
		free(sector.fileSystemType);
		//printf("System shutting down.\n");
	}
	else
	{
		printf("ERROR: File '%s' does not exist.\n", argv[1]);
	}
}

//loads a new directory based on its starting sector, as given from its parent directoryEntry
void loadNewDirectory(int startSector, FILE *file, uint16_t FAT[])
{
	int physicalSector;
	int currentDirPosition = 0;
	do
	{
		physicalSector = logicalToPhysical(startSector);
		readSectorEntries(file, physicalSector, currentDir, currentDirPosition);
		currentDirPosition += 16;
		startSector = getNextSector(startSector, FAT);
	} while (!(startSector >= 0xFF8 && startSector <= 0xFFF) && !(startSector == 0));
	//printDir(currentDir);
}

//loads the root directory into the currentDir structure
void loadRootDirectory(FILE *file)
{
	//start at sector 19
	int i = 0;
	for(i = 19; i < 33; i++)
	{
		int currentDirPosition = (i - 19) * 16;
		readSectorEntries(file, i, rootDir, currentDirPosition);
		readSectorEntries(file, i, currentDir, currentDirPosition);
	}
}

//takes a sector number and reads its contents into directory entries
void readSectorEntries(FILE *file, int sectorNumber, enhancedDirectoryEntry* directory[], int directoryPosition)
{
	int i = 0;
	fseek(file, 512 * sectorNumber, SEEK_SET);
	for(i; i < 16; i++)
	{
		enhancedDirectoryEntry* firstEnt = readEntry(file);
		directory[directoryPosition + i] = firstEnt;
	}
}

//reads a complete directory entry from a file
enhancedDirectoryEntry* readEntry(FILE *file)
{
	if(file != NULL)
	{
			enhancedDirectoryEntry* newEntry = malloc(sizeof(directoryEntry*) + sizeof(unsigned int));
			directoryEntry* temp = malloc(sizeof(directoryEntry) + 11);
			newEntry->entry = temp;
			newEntry->entryLocation = ftell(file);

			temp->fileName = malloc(9);
			temp->extension = malloc(4);
			fread(temp->fileName, 8, 1, diskImage);
			temp->fileName[8] = '\0';
			fread(temp->extension, 3, 1, diskImage);
			temp->extension[3] = '\0';
			fread(&temp->attributes, 1, 1, diskImage);
			fread(&temp->reservedInfo, 2, 1, diskImage);
			fread(&temp->creationTime, 2, 1, diskImage);
			fread(&temp->creationDate, 2, 1, diskImage);
			fread(&temp->lastAccess, 2, 1, diskImage);
			fread(&temp->unusedInfo, 2, 1, diskImage);
			fread(&temp->lastWriteTime, 2, 1, diskImage);
			fread(&temp->lastWriteDate, 2, 1, diskImage);
			fread(&temp->firstLogicalCluster, 2, 1, diskImage);
			fread(&temp->fileSize, 4, 1, diskImage);
			temp->fileName = trimwhitespace(temp->fileName);
			temp->extension = trimwhitespace(temp->extension);

			return newEntry;
	}
	printf("The file was NULL!\n");
	return NULL;
}

//overwrites an existing directory entry structure
int writeEntry(FILE *file, enhancedDirectoryEntry* extEntry)
{
	if(file != NULL)
	{
		if(fseek(file, extEntry->entryLocation, SEEK_SET) == 0)
		{
			char *formattedFileName = padString(extEntry->entry->fileName, 9);
			char *formattedFileExt = padString(extEntry->entry->extension, 4);
			fwrite(extEntry->entry->fileName, 8, 1, diskImage);
			fwrite(extEntry->entry->extension, 3, 1, diskImage);
			fwrite(&extEntry->entry->attributes, 1, 1, diskImage);
			fwrite(&extEntry->entry->reservedInfo, 2, 1, diskImage);
			fwrite(&extEntry->entry->creationTime, 2, 1, diskImage);
			fwrite(&extEntry->entry->creationDate, 2, 1, diskImage);
			fwrite(&extEntry->entry->lastAccess, 2, 1, diskImage);
			fwrite(&extEntry->entry->unusedInfo, 2, 1, diskImage);
			fwrite(&extEntry->entry->lastWriteTime, 2, 1, diskImage);
			fwrite(&extEntry->entry->lastWriteDate, 2, 1, diskImage);
			fwrite(&extEntry->entry->firstLogicalCluster, 2, 1, diskImage);
			fwrite(&extEntry->entry->fileSize, 4, 1, diskImage);
			return 0;
		}
		return -1;
	}
	printf("The file was NULL!\n");
	return -1;
}

//Implementation of cd in the command handler. Will change the directory to the directory specified and load its contents.
int changeDir(char* targetDir, FILE *diskImage, uint16_t FAT[])
{
	if (targetDir != NULL)
	{
		int i = 0;
		for(i; i < ROOT_DIR_SIZE; i++)
		{
			if (isDirectory(currentDir[i]->entry) && !strcmpigncase(currentDir[i]->entry->fileName, targetDir))
			{
				if ((!strcmp(currentDir[i]->entry->fileName, "..")) && currentDir[i]->entry->firstLogicalCluster == 0)
				{
					loadRootDirectory(diskImage);
				}
				else
				{
					loadNewDirectory(currentDir[i]->entry->firstLogicalCluster, diskImage, FAT);
				}
				return 0;
			}
		}
		printf("The desired directory could not be found.\n");
		return -1;
	}
	else
	{
		printf("No directory specified!\n");
		return -1;
	}
}

//checks to see if a given directory entry is itself a subdirectory
int isDirectory(directoryEntry* entry)
{
	return ((entry->attributes & 0x10) != 0);
}

//function to convert from logical sector numbers to physical sector numbers on the fly
int logicalToPhysical(int logicalSectorNumber)
{
	return 31 + logicalSectorNumber;
}

//gets the next sector number by reading the FAT array
int getNextSector(int fatEntry, uint16_t FAT[])
{
	return FAT[fatEntry];
}

//removes white space surrounding a string (i.e. a filename)
char *trimwhitespace(char *str)
{
  char *end;
  
  while(isspace(*str)) str++;
  
  if(*str == 0)
    return str;


  end = str + strlen(str) - 1;
  while(end > str && isspace(*end)) end--;
  
  *(end+1) = 0;
  return str;
}

//performs the inverse operation of trimwhitespace by padding with white space until its total length is that of a full file name (8)
char *padString(char *string, int newLength)
{
  char* newString = malloc(sizeof(char) * newLength);
  int oldLength = strlen(string);
  if (oldLength < newLength) 
  {
    int i = 0;
    int displacement = newLength - oldLength - 1;
    for(; i < displacement; i++)
    {
      newString[i] = ' ';
    }
    for(; i < newLength; i++)
    {
      newString[i] = string[i - displacement];
    }
  }
  else
  {
    free(newString);
    newString = NULL;
  }
  return newString;
}

//displays the raw output of the data stored in a given sector
void printSectorData(int logicalSector)
{
	fseek(diskImage, logicalToPhysical(logicalSector)*512, SEEK_SET);
	char data;
	int i = 0;
	for(i; i < 512; i++)
	{
		fread(&data, 1, 1, diskImage);
		putchar(data);
	}
}


//parses the target path for the changeDir function to execute upon
char** getTargetPath(char* target)
{
	char **targetPath = malloc(sizeof(char *) * MAX_PATH_SIZE);
	char *path = strtok(target, "/");
	int i = 0;
	do
	{
		targetPath[i] = path;
		i++;
	} while(i < MAX_PATH_SIZE && ((path = strtok(NULL, "/")) != NULL));
	targetPath[i] = NULL;
	return targetPath;
}