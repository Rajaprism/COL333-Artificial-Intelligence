#include "types.h"
#include "defs.h"
#include "x86.h"
#include "mouse.h"
#include "traps.h"


#define DATA_PORT 0x60
#define CMD_PORT  0x64
#define PORT_BSY 2   // 1st bit set showing can't send anything
#define PORT_READ 1  // 0th bit set showing can read now
#define MOUSE 0xD4
#define ACK 0xFA
#define COMPAQ_CMD_BYTE 0x20
#define AUX_CMD 0xA8
#define DEF_SETTING 0xF6
#define ENB_PKT_STR 0xF4 

#define IRQ_MOUSE 12
#define CPU_0 0
#define LEFT 0x1
#define RIGHT 0x2
#define MIDDLE 0x4
// Wait until the mouse controller is ready for us to send a packet
void 
mousewait_send(void) 
{
    // Implement your code here
    while(inb(CMD_PORT) & PORT_BSY)
    return;
}

// Wait until the mouse controller has data for us to receive
void 
mousewait_recv(void) 
{
    // Implement your code here
    uchar r;
    while(((r=inb(CMD_PORT)) & PORT_READ) != PORT_READ);
    return;
}



// Send a one-byte command to the mouse controller, and wait for it
// to be properly acknowledged
void 
mousecmd(uchar cmd) 
{
    // Implement your code here
    mousewait_send();
    outb(CMD_PORT,MOUSE);
    // mousewait_recv();
    // while((inb(DATA_PORT) & ACK) != ACK);

    mousewait_send();
    outb(DATA_PORT,cmd);
    mousewait_recv(); 

    uchar ack = inb(DATA_PORT); 
    if (ack != ACK) {
        cprintf("Mouse command acknowledgment error: %x\n", ack);
    }

    return;
}

void
mouseinit(void)
{
    // Implement your code here
    // enable AUX port
    mousewait_send();
    outb(CMD_PORT,AUX_CMD);
    // mousewait_recv();
    // while((inb(DATA_PORT) & ACK) != ACK);  // given in wiki that it generate ack

    mousewait_send();
    outb(CMD_PORT,COMPAQ_CMD_BYTE);
    mousewait_recv();

    int Compaq_status_byte = inb(DATA_PORT) | 0x02;

    mousewait_send();
    outb(CMD_PORT,0x60);

    mousewait_send();
    outb(DATA_PORT,Compaq_status_byte);

    mousecmd(DEF_SETTING);

    mousecmd(ENB_PKT_STR);

    ioapicenable(IRQ_MOUSE,CPU_0);  // enable interrupt at IRQ_12
    cprintf("Mouse has been initialized\n");
    return;
}

void
mouseintr(void)
{
    uchar Click=0;

    while(inb(CMD_PORT) & PORT_READ){
        mousewait_recv();
        Click = inb(DATA_PORT);
        if(Click & LEFT  ) cprintf("LEFT\n");
        if(Click & RIGHT ) cprintf("RIGHT\n");
        if(Click & MIDDLE) cprintf("MID\n");
        mousewait_recv();
        inb(DATA_PORT);
        mousewait_recv();
        inb(DATA_PORT);
    }

    // Implement your code here
    
    return;

}