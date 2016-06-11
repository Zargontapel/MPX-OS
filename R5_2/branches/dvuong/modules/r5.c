#include "r5.h"
#include <mem/heap.h>
#include <core/serial.h>
#include "string.h"

MemoryBlockList freeList;
MemoryBlockList allocList;

int init_heap(int size)
{
        CompleteMCB* heap = (CompleteMCB*)kmalloc(sizeof(CompleteMCB) + size + sizeof(LimitedMCB));
	heap->type = 0;
	heap->beginningAddress = (u32int)&heap;
	heap->size = size;
	heap->ownerName = "HEAP";
	heap->next = NULL;
	heap->previous = NULL;

	freeList.head = heap;
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

  do {
    curSize = pointer->size + sizeof(CompleteMCB) + sizeof(LimitedMCB);
    if (curSize >= modSize) {
      //Temporarily Unlink from free list
      if (pointer->previous != NULL)
	pointer->previous->next = pointer->next;
      if (pointer->next != NULL)
	pointer->next->previous = pointer->previous;
      
      pointer->next = NULL; pointer->previous = NULL;
	
      //Determine if we are dividing
      if (curSize - modSize < threshold)
	allocSize = curSize;
      else
	allocSize = size;

      //Set the block to allocated
      pointer->type = 1;
      pointer->size = allocSize - sizeof(CompleteMCB) - sizeof(LimitedMCB);
      pointer->ownerName = "NAME";

      //Insert the allocated block into the list
      insertMCB(allocList, pointer);
      
      //Create the allocated block end
      allocatedBlockEnd = (LimitedMCB*)(pointer->beginningAddress + sizeof(CompleteMCB) + allocSize);
      allocatedBlockEnd->type = 1;
      allocatedBlockEnd->size = allocSize;

      //The threshold is large enough to divide into another Free MCB
      if (allocSize != curSize) {
	CompleteMCB* remainder = (CompleteMCB*)(pointer->beginningAddress + modSize);
	serial_println("No page fault yet");
	remainder->type = 0;
	remainder->beginningAddress = pointer->beginningAddress + modSize;
	remainder->size = curSize - modSize - sizeof(CompleteMCB) - sizeof(LimitedMCB);
	remainder->ownerName = "FREE";
	remainder->next = NULL;
	remainder->previous = NULL;
	serial_println("remainder members have been set");
	insertMCB(freeList, remainder);
	serial_println("insertMCB worked correctly");
	LimitedMCB* remainderEnd = (LimitedMCB*)(pointer->beginningAddress + sizeof(CompleteMCB) + pointer->size);
	remainderEnd->type = 0;
	serial_println("type has been set");
	remainderEnd->size = modSize - sizeof(CompleteMCB) - sizeof(LimitedMCB);
	serial_println("if statemement complete");
      }

      success = 1;
      break;
    }
    pointer = pointer->next;
  } while (pointer != NULL);

  if (success == 1)
    return (u32int)pointer;
  else
    return NULL;
}

void insertMCB(MemoryBlockList list, CompleteMCB* block) {
  if (list.head == NULL) {
    list.head = block;
  }
  else if (list.head->beginningAddress > block->beginningAddress) {   
    block->next = list.head;
    list.head->previous = block;
    list.head = block;
  }
  else {
    CompleteMCB* pointer = list.head->next;
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
  CompleteMCB* freePointer = freeList.head;
  LimitedMCB* freeEnd;

  int success = -1;
  do {
    if (pointer->beginningAddress == (u32int)p) {
      if (pointer->previous != NULL)
	pointer->previous->next = pointer->next;
      if (pointer->next != NULL)
	pointer->next->previous = pointer->previous;
      pointer->next = NULL; pointer->previous = NULL;

      //Set the MCB pointer to Free
      pointer->type = 0;

      do {
	freePointer = freePointer->next;
      } while (freePointer != NULL && freePointer->beginningAddress < pointer->beginningAddress);

      //Merge Free blocks
      if (freePointer->beginningAddress - pointer->beginningAddress == 1) {
	if (freePointer->previous != NULL)
	  freePointer->previous->next = freePointer->next;
	if (freePointer->next != NULL)
	  freePointer->next->previous = freePointer->previous;
	freePointer->next = NULL; freePointer->previous = NULL;
	
	pointer->size = pointer->size + sizeof(CompleteMCB) + freePointer->size;
      }

      insertMCB(freeList, pointer);

      //Modify the ending MCB if necessary
      freeEnd = (LimitedMCB*)(pointer->beginningAddress + sizeof(CompleteMCB) + pointer->size);
      freeEnd->type = 0;
      freeEnd->size = pointer->size;
      success = 1;
      break;
    }

    pointer = pointer->next;
  } while (pointer != NULL);

  return success;
}

int is_empty(void)
{
	return (freeList.head != NULL && freeList.head->next == NULL);
}

void print_alloc(void)
{
	CompleteMCB* currMCB = allocList.head;
	int i = 0;
	while(currMCB != NULL)
	{
		serial_print("Block ");
		serial_println(itoa(i));
		serial_print("    Offset: ");
		serial_println(itoa(currMCB->beginningAddress - allocList.head->beginningAddress));
		serial_print("    Size: ");
		serial_println(itoa(currMCB->size));
		currMCB = currMCB->next;
		i++;
	}
}

void print_free(void)
{
	CompleteMCB* currMCB = freeList.head;
	int i = 0;
	while(currMCB != NULL)
	{
		serial_print("Block ");
		serial_println(itoa(i));
		serial_print("    Offset: ");
		serial_println(itoa(currMCB->beginningAddress - allocList.head->beginningAddress));
		serial_print("    Size: ");
		serial_println(itoa(currMCB->size));
		currMCB = currMCB->next;
		i++;
	}
}
