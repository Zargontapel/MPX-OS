#include "r6.h"
#include "com_handler.h"
//main function to invoke the command line at startup
void commandLine(FILE *diskImage, uint16_t FAT[])
{
	char* command = malloc(32);
	char* input = malloc(100);
	char buffer;
	printf("\nWelcome to the FAT12 Explorer.\n");
	printf("Feel free to begin entering commands.\n");
	printf("->");
	while(1) {
		fgets(input, 100, stdin);
		if ((input != NULL))
		        input[strlen (input) - 1] = ' ';
		command = strtok(input, " ");	
		if(command != NULL)
		{
			if (!strcmp(command, "shutdown")) {
				if (shutdownConfirmed()) {
					break;
				} else {
					printf("Shutdown canceled.\n");
				}
			} else if (!strcmp(command, "bootinfo")) {
				bootinfo();
			} else if (!strcmp(command, "root")) {
				printroot();
			} else if (!strcmp(command, "cd")) {
				command = strtok(NULL, " ");
				changeDir(command, diskImage, FAT);
			} else if (!strcmp(command, "ls")) {
				char* name = strtok(NULL, " ");
				list(name);
			} else if (!strcmp(command, "type")) {
				char* name = strtok(NULL, " ");
				type(FAT, name);
			} else if (!strcmp(command, "rn")) {
				char* target = strtok(NULL, " ");
				char* newName = strtok(NULL, " ");
				relabel(target, newName, diskImage, FAT);
			} else if (!strcmp(command, "help")) {
			        help();
			} else {
				printf("Command '%s' not recognized. Type help to view commands.\n", command);
			}
		}
		printf("->");
	}
	free(command);
}

//tells the command line function when the user decides to shut down the system
int shutdownConfirmed()
{
	printf("Are you sure you would like to shutdown? (Y/N)\n");
	char* response = malloc(4);
	scanf("%s", response);
	if (!strcmpigncase(response, "y") || !strcmpigncase(response, "yes")) {
		free(response);
		return 1;
	} else if (!strcmpigncase(response, "n") || !strcmpigncase(response, "no")) {
		free(response);
		return 0;
	} else {
		printf("Entered value not recognized.\n");
		return 0;
	}
}

//prints all relevant boot sector info after it has been loaded from the disk
void bootinfo()
{
	printf("\nBytes per sector: %u\n", sector.bytesPerSector);
	printf("Sectors per cluster: %u\n", sector.sectorsPerCluster);
	printf("Number of reserved sectors: %u\n", sector.reservedSectors);
	printf("Number of FAT copies: %u\n", sector.fatCopies);
	printf("Maximum number of root directory entries: %u\n", sector.maxRootDirEntries);
	printf("Total number of sectors: %u\n", sector.numSectors);
	printf("Number of sectors per FAT: %u\n", sector.sectorsPerFAT);
	printf("Number of sectors per track: %u\n", sector.sectorsPerTrack);
	printf("Number of heads: %u\n", sector.numHeads);
	printf("Volume ID: %u\n", sector.volumeID);
	printf("Volume label: %s\n", sector.volumeLabel);
	printf("File system type: %s\n", sector.fileSystemType);
}

//prints all stil-present files in the root directory. Can be called from any directory
void printroot()
{
	int i = 0;
	printf("\nFILE\t\t\tATTRIBUTES\t\tFIRST CLUSTER\t\tSIZE\n");
	printf("----------------------------------------------------------------------------\n\n");
	for(i; i < 224; i++)
	{
		if(((unsigned char)rootDir[i]->entry->fileName[0] != 0xE5) && (rootDir[i]->entry->attributes != 0x28))
		{
			if(rootDir[i]->entry->fileName[0] != 0i)
			{
				if (isDirectory(rootDir[i]->entry))
				{
					printf("%s%-5s", rootDir[i]->entry->fileName, "");
				}
				else {
					printf("%s.%-5s", rootDir[i]->entry->fileName, rootDir[i]->entry->extension);
				}
				printf("\t\t%u", rootDir[i]->entry->attributes);
				printf("\t\t\t%u\t\t\t%lu\n", rootDir[i]->entry->firstLogicalCluster, rootDir[i]->entry->fileSize);
			}
			if(rootDir[i]->entry->fileName[0] == 0)
			{
				i = 225;
			}
		}
	}
}

//changes to a new directory, and loads in all the new directory entries of that directory
void cd(char* targetDir, FILE *diskImage, uint16_t FAT[])
{
	changeDir(targetDir, diskImage, FAT);
}

//prints all still-existing files in the current directory
void list(char* name)
{
	int i = 0;
	if(name == NULL)
	{
		printf("\nFILE\t\t\tATTRIBUTES\t\tFIRST CLUSTER\t\tSIZE\n");
		printf("----------------------------------------------------------------------------\n\n");
		for(i; i < 224; i++)
		{
			if(((unsigned char)currentDir[i]->entry->fileName[0] != 0xE5) && (currentDir[i]->entry->attributes != 0x28))
			{
				if(currentDir[i]->entry->fileName[0] != 0)
				{
					if (isDirectory(currentDir[i]->entry))
					{
						printf("%s%-6s", currentDir[i]->entry->fileName, currentDir[i]->entry->extension);
					}
					else {
						printf("%s.%-5s", currentDir[i]->entry->fileName, currentDir[i]->entry->extension);
					}
					printf("\t\t%u", currentDir[i]->entry->attributes);
					printf("\t\t\t%u\t\t\t%lu\n", currentDir[i]->entry->firstLogicalCluster, currentDir[i]->entry->fileSize);
				}
				if(currentDir[i]->entry->fileName[0] == 0)
				{
					i = 225;
				}
			}
		}
	}
	else
	{
		char* fileName = strtok(name, ".");
		char* extension = strtok(NULL, ".");
		printf("\nFILE\t\t\tATTRIBUTES\t\tFIRST CLUSTER\t\tSIZE\n");
		printf("--------------------------------------------------------------------------\n\n");
		if(strcmp(fileName, "*") && strcmp(extension, "*"))
		{
			for(i; i < 224; i++)
			{
				if(!strcmp(currentDir[i]->entry->fileName, fileName) && !strcmp(currentDir[i]->entry->extension, extension))
				{
					printf("%s.%s   ", currentDir[i]->entry->fileName, currentDir[i]->entry->extension);
					printf("\t\t%u", currentDir[i]->entry->attributes);
					printf("\t\t\t%u\t\t\t%lu\n", currentDir[i]->entry->firstLogicalCluster, currentDir[i]->entry->fileSize);
				}
			}
		}
		else if(strcmp(fileName, "*"))
		{
			for(i; i < 224; i++)
			{
				if(!strcmp(currentDir[i]->entry->fileName, fileName))
				{
					printf("%s.%s   ", currentDir[i]->entry->fileName, currentDir[i]->entry->extension);
					printf("\t\t%u", currentDir[i]->entry->attributes);
					printf("\t\t\t%u\t\t\t%lu\n", currentDir[i]->entry->firstLogicalCluster, currentDir[i]->entry->fileSize);
				}
			}
		}
		else if(strcmp(extension, "*"))
		{
			for(i; i < 224; i++)
			{
				if(!strcmp(currentDir[i]->entry->extension, extension))
				{
					printf("%s.%s   ", currentDir[i]->entry->fileName, currentDir[i]->entry->extension);
					printf("\t\t%u", currentDir[i]->entry->attributes);
					printf("\t\t\t%u\t\t\t%lu\n", currentDir[i]->entry->firstLogicalCluster, currentDir[i]->entry->fileSize);
				}
			}
		}
	}
}

//outputs the raw contents of the specified file to the terminal
void type(uint16_t FAT[], char* name)
{
	if (endsWith(name, ".TXT") == 0 && 
	    endsWith(name, ".BAT") == 0 &&
	    endsWith(name, ".C") == 0) {
		printf("%s cannot be displayed.\n", name);
		return;
	}

	strtok(name, ".");

	//printf("Now printing a file of type .c .txt or .bat\n");
	int i = 0;
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
				printf("\nPress enter to continue.\n");
				fgets(entry, 32, stdin);
				printSectorData(sector);
				//printf("\nPress enter to continue.\n");
				//fgets(entry, 32, stdin);
				sector = FAT[sector];
				value = sector;
			}while(FAT[sector] < 0xFF0);
			printSectorData(sector);
			printf("\n");
			fileFound = 1;
			break;
		}
	}
	if(!fileFound)
	printf("File not found.\n");
}

//changes the fileName member of the given file's directoryEntry structure
void relabel(char* target, char* newName, FILE* diskImage, uint16_t FAT[])
{
	if (target != NULL && newName != NULL)
	{
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
		char **oldNameTokens = tokenizeName(targetPath[i]);
		char **nameTokens = tokenizeName(newName);
		if (nameTokens[0] == NULL)
		{
			printf("The format of the new name is not allowable.\n");
			printf("The preferred format is: <NAME>.<EXT>\n");
			printf("Where <NAME> can be up to 8 characters in length, and <EXT> can be up to 3 characters in length.\n");
			return;
		}
		int j = 0;
		for(; j < ROOT_DIR_SIZE; j++)
		{
			if (currentDir[j]->entry->fileName[0] != 0xe5 && !strcmp(currentDir[j]->entry->fileName, oldNameTokens[0]) && !strcmp(currentDir[j]->entry->extension, oldNameTokens[1]))
			{
				free(currentDir[j]->entry->fileName);
				free(currentDir[j]->entry->extension);
				char *token1 = malloc(strlen(nameTokens[0]) + 1);
				char *token2 = malloc(strlen(nameTokens[1]) + 1);
				strcpy(token1, nameTokens[0]);
				strcpy(token2, nameTokens[1]);
				currentDir[j]->entry->fileName = token1;
				currentDir[j]->entry->extension = token2;
				if (currentDir[0]->entry->attributes == 0x28)
				{
					rootDir[j]->entry->fileName = token1;
					rootDir[j]->entry->extension = token2;
				}
				writeEntry(diskImage, currentDir[j]);
				printf("The file has been renamed.\n");
			}
		}
		free(targetPath);
	}
	else
	{
		printf("Invalid format provided for renaming of a file.\n");
	}
}

//separates the full file name into its actual file name, and its extension
char** tokenizeName(char* name)
{
	char **nameTokens = malloc(sizeof(char *) * 2);
	char *fileName = strtok(name, ".");
	char *fileExtension = strtok(NULL, ".");
	if ((strlen(fileName) <= MAX_NAME_LENGTH + 1) && (strlen(fileExtension) <= MAX_EXT_LENGTH + 1))
	{
		toUpper(fileName);
		nameTokens[0] = fileName;
		if (fileExtension != NULL)
		{
			toUpper(fileExtension);
			nameTokens[1] = fileExtension;
		}
		else
		{
			nameTokens[1] = "";
		}
	}
	else
	{
		nameTokens[0] = NULL;
		nameTokens[1] = NULL;
	}
	return nameTokens;
}

//displays a listing of all function the user can execute and their purposes
void help() {
       printf("\nCurrent commands: \nshutdown : \tShuts down the program.\n");
       printf("bootinfo : \tPrints the boot sector information.\n");
       printf("root : \t\tLists all the files and directories in the  root directory.\n");
       printf("cd : \t\tChanges the current directory to the given directory.\n");
       printf("ls : \t\tLists the entire contents of the current directory or given set of files.\n");
       printf("type : \t\tPrints the contents of any file with the extension .txt, .bat, or .c\n");
       printf("rn : \t\tRename a file to a new given name of 8 characters and 3 character exension.\n");
       printf("help : \t\tDisplays this help information.\n");
}
