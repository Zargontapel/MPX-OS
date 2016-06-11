typedef struct Context {
  u32int gs, fs, es, ds;
  u32int edi, esi, ebp, esp, ebr, edr, ecx, esr;
  u32int eip, cs, uflags;
} context;
