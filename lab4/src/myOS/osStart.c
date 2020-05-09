#include "include/i8253.h"
#include "include/i8259.h"
#include "include/irq.h"
#include "include/uart.h"
#include "include/vga.h"
#include "include/mem.h"
#include "include/myPrintk.h"

extern void myMain(void);

void pressAnyKeyToStart(void){
    myPrintk(PURPLE,"Prepare uart device\n");
    myPrintk(PURPLE,"Then, press any key to start ...\n");

    uart_get_char();
}

void osStart(void){
    pressAnyKeyToStart(); // prepare for uart device
    init8259A();
    init8253();
    enable_interrupt();

    clear_screen();

    pMemInit();  //after this, we can use kmalloc/kfree and malloc/free

    {
        unsigned long tmp = dPartitionAlloc(pMemHandler,100);
        dPartitionWalkByAddr(pMemHandler);
        dPartitionFree(pMemHandler,tmp);
        dPartitionWalkByAddr(pMemHandler);
    }

    // finished kernel init
    // NOW, run userApp
    myPrintk(DARK_GREEN,"START RUNNING......\n");    
    myMain();
    myPrintk(DARK_GREEN, "STOP RUNNING......ShutDown\n");
    while(1);
}
