#include "printk/myPrintk.h"
#include "lib/color.h"

extern void clear_screen(void);
extern void myMain(void);
extern void init8259A(void);
extern void init8253(void);
extern void setWallClock(int h, int m, int s);
extern void enable_interrupt(void);

void osStart(void)
{
	enable_interrupt();
	init8259A();
	init8253();
	clear_screen();
	setWallClock(0, 0, 0);
	myPrintk(DARK_GREEN, "START RUNNING......\n");
	myMain();
	myPrintk(DARK_GREEN, "STOP RUNNING......ShutDown\n");
	while (1);
}
