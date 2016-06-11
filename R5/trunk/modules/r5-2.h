typedef unsigned long long u32int;

typedef struct cmcb{
  //0 - Free
  //1 - Allocated
  unsigned char type;
  u32int beginningAddress;
  u32int size;
  char* ownerName;
  
  struct cmcb* next;
  struct cmcb* previous;
} CompleteMCB;

typedef struct {
  //0 - Free
  //1 - Allocated
  unsigned char type;
  u32int size;
} LimitedMCB;

typedef struct {
  CompleteMCB* head;
  CompleteMCB* tail;
} MemoryBlockList;

/*
  Procedure..: init_heap
  Description..: Initializes the heap to a given size
  Params..: The size to initialize the heap to
  Returns..: The the size if successful, -1 if not
*/
int init_heap(int size);

/*
  Procedure..: my_alloc
  Description..: Allocates a given block of memory
  Params..: The size to allocate
  Returns..: The the beginning address of the writeable memory if successful, -1 if not
*/
u32int my_alloc(u32int size);

/*
  Procedure..: insertMCB
  Description..: Inserts a given Memory Control Block into a given memory list
  Params..: The list to insert the MCB into and the block of memory to insert
*/
void insertMCB(MemoryBlockList* list, CompleteMCB* block);

/*
  Procedure..: my_free
  Description..: Frees a given block of memory
  Params..: The pointer to a block of memory
  Returns..: 1 if successful, -1 if not
*/
int my_free(void *p);

/*
  Procedure..: print_alloc
  Description..: Prints the list of allocated memory blocks
*/
void print_alloc(void);

/*
  Procedure..: print_free
  Description..: Prints the list of free memory blocks
*/
void print_free(void);

/*
  Procedure..: is_empty
  Description..: Checks if the heap is empty
  Returns..: Returns 1 if empty, else 0
*/
int is_empty(void);

/*
  Procedure..: cleanupList
  Description..: Cleans up the heap, merging contiguous free blocks
*/
void cleanupList(void);

/*
  Procedure..: merge
  Description..: Merges two MCB blocks
*/
void merge(CompleteMCB*, CompleteMCB*);
