/*
  ----- kmain.c -----

  Description..: Kernel main. The first function called after
      the bootloader. Initialization of hardware, system
      structures, devices, and initial processes happens here.
*/

#include <stdint.h>
#include <string.h>
#include <system.h>

#include <core/io.h>
#include <core/serial.h>
#include <core/tables.h>
#include <core/interrupts.h>
#include <mem/heap.h>
#include <mem/paging.h>

#include "modules/mpx_supt.h"
#include "modules/com_handler.h"

void kmain(void)
{
   //extern uint32_t magic;
   // Uncomment if you want to access the multiboot header
   // extern void *mbd;
   // char *boot_loader_name = (char*)((long*)mbd)[16];

   // 0) Initialize Serial I/O and call mpx_init
   init_serial(COM1);
   set_serial_in(COM1);
   set_serial_out(COM1);
   mpx_init(MODULE_R2);
   klogv("Starting MPX boot sequence...");
   klogv("Initialized serial I/O on COM1 device...");

   // 1) Check that the boot was successful and correct when using grub
   // Comment this when booting the kernel directly using QEMU, etc.
   //if ( magic != 0x2BADB002 ){
     //kpanic("Boot was not error free. Halting.");
   //}
   
   // 2) Descriptor Table
   // Initialize Global Descriptor Table
   init_gdt();
   // Initialze Interrupt Descriptor Table
   init_idt();
   
   // Initialize Programmable Interrupt Controllers
   init_pic();
   // Initialize Interrupt Handlers
   init_irq();

   // Enable Interrupts
   sti();
   klogv("Initializing descriptor tables...");

   // 4) Virtual Memory
   // Initialize Paging
   init_paging();
   klogv("Initializing virtual memory...");

   // 5) Call Commhand
   klogv("Transferring control to commhand...");
   comhandle();

   // 11) System Shutdown
   klogv("Starting system shutdown procedure...");
   
   /* Shutdown Procedure */
   klogv("Shutdown complete. You may now turn off the machine. (QEMU: C-a x)");
   hlt();
}
