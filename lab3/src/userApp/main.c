extern int myPrintk(int color, const char* format, ...);
extern void append2screenAtpoint(int line, int column, char* str, int color);
extern int getTickCount();
extern int startShell();

void myMain(void)
{
	while(1)
	    startShell();
}
