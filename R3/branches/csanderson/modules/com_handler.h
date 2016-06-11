#include <system.h>
#include <string.h>
#include "polling.h"
#include "date_time.h"
#include "procsr3.h"

void comhandle();
int shutdownConfirmed();
void version();
void help();
void setdate();
void getdate();
void settime();
void gettime();
void suspendPCB();
void resumePCB();
void setPriority();
void showPCB();
void showReady();
void showBlocked();
void showAll();
void deletePCB();
void block();
void unblock();
void loadR3();
void loadBasicFunctionToPCB(char[], unsigned char, unsigned char, unsigned char, unsigned char, int, void (*func)());