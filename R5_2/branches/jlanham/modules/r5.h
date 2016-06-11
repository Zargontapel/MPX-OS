#include <system.h>

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

int init_heap(int size);

u32int my_alloc(u32int size);
void insertMCB(MemoryBlockList* list, CompleteMCB* block);

int my_free(void *p);

void print_alloc(void);

void print_free(void);

int is_empty(void);

void cleanupList(void);

void merge(CompleteMCB*, CompleteMCB*);
