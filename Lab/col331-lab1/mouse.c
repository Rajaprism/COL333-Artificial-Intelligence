#include "types.h"
#include "defs.h"
#include "x86.h"
#include "mouse.h"
#include "traps.h"

// Wait until the mouse controller is ready for us to send a packet
void 
mousewait_send(void) 
{
    // Implement your code here
    return;
}

// Wait until the mouse controller has data for us to receive
void 
mousewait_recv(void) 
{
    // Implement your code here
    return;
}

// Send a one-byte command to the mouse controller, and wait for it
// to be properly acknowledged
void 
mousecmd(uchar cmd) 
{
    // Implement your code here
    return;
}

void
mouseinit(void)
{
    // Implement your code here
    return;
}

void
mouseintr(void)
{
    // Implement your code here
    return;
}