#include "r5-2.h"
#include "string.h"
#include <stdio.h>
#include <stdlib.h>
typedef unsigned long long u32int;

MemoryBlockList freeList = {NULL, NULL};
MemoryBlockList allocList = {NULL, NULL};
u32int heapBeginning = 0;

int init_heap(int size)
{
  CompleteMCB* heap = (CompleteMCB*)malloc(sizeof(CompleteMCB) + size + sizeof(LimitedMCB));
  heap->type = 0;
  heap->beginningAddress = (u32int)&heap;
  heapBeginning = heap->beginningAddress + sizeof(CompleteMCB);
  heap->size = (u32int)size;
  heap->ownerName = "HEAP";
  heap->next = NULL;
  heap->previous = NULL;

  insertMCB(&freeList, heap);
  allocList.head = NULL;
  LimitedMCB* heapEnd = (LimitedMCB*)heap->beginningAddress + sizeof(CompleteMCB) + size;
  heapEnd->type = 0;
  heapEnd->size = size;

  if(heap != NULL && heapEnd != NULL)
  {
    return size;
  }
  else
  {
    return -1;
  }	
}

u32int my_alloc(u32int size) {
  u32int modSize = size + sizeof(CompleteMCB) + sizeof(LimitedMCB);

  //Round up to next word size
  u32int wordSizeFactor = (modSize + 31) / 32;
  modSize = wordSizeFactor * 32;

  u32int curSize;
  u32int allocSize;

  CompleteMCB* pointer = freeList.head;
  LimitedMCB* allocatedBlockEnd;
  
  u32int threshold = sizeof(CompleteMCB) + sizeof(LimitedMCB) + 50;
  unsigned char success = 0;

  while(pointer != NULL){
    curSize = pointer->size + sizeof(CompleteMCB) + sizeof(LimitedMCB);
    if (curSize >= modSize) {
      //Temporarily Unlink from free list
      if (pointer->previous != NULL)
       pointer->previous->next = pointer->next;
     else
      freeList.head = pointer->next;    
    if (pointer->next != NULL)
     pointer->next->previous = pointer->previous;

   pointer->next = NULL; pointer->previous = NULL;

      //Determine if we are dividing
   if (curSize - modSize < threshold)
     allocSize = curSize;
   else
     allocSize = modSize;

      //Set the block to allocated
   pointer->type = 1;
   pointer->size = allocSize - sizeof(CompleteMCB) - sizeof(LimitedMCB);
   pointer->ownerName = "NAME";

      //Insert the allocated block into the list
   insertMCB(&allocList, pointer);

      //Create the allocated block end
   allocatedBlockEnd = (LimitedMCB*)(pointer->beginningAddress + allocSize - sizeof(LimitedMCB));
   allocatedBlockEnd->type = 1;
   allocatedBlockEnd->size = pointer->size;

      //The threshold is large enough to divide into another Free MCB
   if (allocSize != curSize) {
     CompleteMCB* remainder = (CompleteMCB*)(pointer->beginningAddress + modSize);
     remainder->type = 0;
     remainder->beginningAddress = pointer->beginningAddress + modSize;
     remainder->size = curSize - modSize - sizeof(CompleteMCB) - sizeof(LimitedMCB);
     remainder->ownerName = "FREE";
     remainder->next = NULL;
     remainder->previous = NULL;
     insertMCB(&freeList, remainder);
     LimitedMCB* remainderEnd = (LimitedMCB*)(remainder->beginningAddress + sizeof(CompleteMCB) + remainder->size);
     remainderEnd->type = 0;
     remainderEnd->size = remainder->size;
   }

   success = 1;
   break;
 }
 pointer = pointer->next;
}

if (success == 1) {
  return pointer->beginningAddress + sizeof(CompleteMCB);
}
else
  return -1;
}

void insertMCB(MemoryBlockList* list, CompleteMCB* block) {
  if (list->head == NULL) {
    list->head = block;
  }
  else if (list->head->beginningAddress > block->beginningAddress) {   
    block->next = list->head;
    list->head->previous = block;
    list->head = block;
  }
  else {
    CompleteMCB* pointer = list->head->next;
    CompleteMCB* oldPointer;
    do {
      oldPointer = pointer;
      pointer = pointer->next;
    }
    while (pointer->beginningAddress < block->beginningAddress);

    pointer = oldPointer;	
    block->next = pointer->next;
    block->previous = pointer;
    pointer->next->previous = block;
    pointer->next = block;

  }
}

int my_free(void *p) {
  CompleteMCB* pointer = allocList.head;
  LimitedMCB* freeEnd = NULL;

  int success = -1;
  while(pointer != NULL) {
    if (pointer->beginningAddress == (u32int)(p - sizeof(CompleteMCB))) {
      if (pointer->previous != NULL)
       pointer->previous->next = pointer->next;
     else
     {
      allocList.head = pointer->next;
    }
    if (pointer->next != NULL)
     pointer->next->previous = pointer->previous;
   pointer->next = NULL; pointer->previous = NULL;

      //Set the MCB pointer to Free
   pointer->type = 0;

   insertMCB(&freeList, pointer);

      //Modify the ending MCB if necessary
   freeEnd = (LimitedMCB*)(pointer->beginningAddress + sizeof(CompleteMCB) + pointer->size);
   freeEnd->type = 0;
   freeEnd->size = pointer->size;
   cleanupList();
   success = 1;
   break;
 }

 pointer = pointer->next;
}

return success;
}

int is_empty(void)
{
	return (freeList.head != NULL && freeList.head->next == NULL && allocList.head == NULL);
}

void print_alloc(void)
{
	CompleteMCB* currMCB = allocList.head;
	int i = 1;
	while(currMCB != NULL)
	{
		printf("Block ");
		printf("%d\n", i);
		printf("    Offset: ");
		printf("%lld\n", currMCB->beginningAddress + sizeof(CompleteMCB) - heapBeginning);
		printf("    Size: ");
		printf("%lld\n", currMCB->size);
		currMCB = currMCB->next;
		i++;
	}
}

void print_free(void)
{
	CompleteMCB* currMCB = freeList.head;
	int i = 1;
	while(currMCB != NULL)
	{
		printf("Block ");
		printf("%d\n", i);
		printf("    Offset: ");
		printf("%lld\n", currMCB->beginningAddress + sizeof(CompleteMCB) - heapBeginning);
		printf("    Size: ");
		printf("%lld\n", currMCB->size);
		currMCB = currMCB->next;
		i++;
	}
}

void cleanupList()
{
	CompleteMCB* pointer = freeList.head;
	while(pointer->next != NULL)
	{
		if(pointer->previous != NULL)
		{
			LimitedMCB* previousTag = (LimitedMCB*)(pointer->beginningAddress - sizeof(LimitedMCB));
			if(previousTag->type == 0)
			{
				CompleteMCB* previousMCB = (CompleteMCB*)(pointer->beginningAddress - sizeof(LimitedMCB) - sizeof(CompleteMCB) - previousTag->size);
				merge(previousMCB, pointer);
			} 	
		}
		if(pointer->next != NULL)
		{
			CompleteMCB* nextTag = (CompleteMCB*)(pointer->beginningAddress + sizeof(CompleteMCB) + sizeof(LimitedMCB) + pointer->size);
			if(nextTag->type == 0)
			{
				merge(pointer, nextTag);
			}
		}
		pointer = pointer->next;
	}
}

void merge(CompleteMCB* dest, CompleteMCB* src)
{
	dest->size += sizeof(LimitedMCB) + sizeof(CompleteMCB) + src->size;
	dest->next = src->next;
}
